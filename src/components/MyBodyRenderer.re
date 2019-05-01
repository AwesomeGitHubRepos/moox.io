open BsReactNative;

type htmlProps = {
  .
  "id": string,
  "name": string,
  "for": string,
  "class": string,
  "href": string,
};

type reasonChildren = array(reasonChild)
and reasonChild =
  | String(string)
  | Element(string, htmlProps, reasonChildren)
  | Empty;

type jsBody = {
  .
  "t": string,
  "p": htmlProps,
  "c": Js.Undefined.t(array(jsBody)),
};

let rec jsTreeToReason = (jsChild: jsBody) =>
  switch ([%bs.raw {| Object.prototype.toString.call(jsChild) |}]) {
  | "[object String]" => String(Js.String.make(jsChild))
  | "[object Object]" =>
    let tag = Js.String.make(jsChild##t);
    let props = jsChild##p;
    let children =
      switch (Js.Undefined.toOption(jsChild##c)) {
      | Some(c) => c->Belt.Array.map(jsTreeToReason)
      | None => [||]
      };
    Element(tag, props, children);
  | _ => Empty
  };

let string_map_partial = (f, s) => {
  let b = Buffer.create(String.length(s));
  let addperhaps = c =>
    switch (f(c)) {
    | None => ()
    | Some(c') => Buffer.add_char(b, c')
    };
  String.iter(addperhaps, s);
  Buffer.contents(b);
};

let lastSiblingHasLineBreaking = ref(false);

let rec renderChild = (parentTag, index: int, child) => {
  /* @todo we can do better */
  let key = index->string_of_int;
  let siblingHasLineBreaking = lastSiblingHasLineBreaking^;
  lastSiblingHasLineBreaking := false;
  let renderChildren = (parentTag, children) =>
    if (children->Belt.Array.length == 0) {
      React.null;
    } else {
      children->Belt.Array.mapWithIndex(renderChild(parentTag))->React.array;
    };
  switch (child) {
  | String(string) =>
    switch (parentTag) {
    | "ul" => React.null
    | "ol" => React.null
    | _ =>
      let newString =
        string_map_partial(
          char =>
            if (char === '\n' && siblingHasLineBreaking) {
              None;
            } else {
              Some(char === '\n' ? ' ' : char);
            },
          string,
        );
      newString->React.string;
    }
  | Element(tag, props, children) =>
    switch (tag) {
    /* custom tag */
    | "huge" => <Huge key> {renderChildren(tag, children)} </Huge>
    /* html tags */
    | "a" =>
      <UnderlinedTextLink key href=props##href>
        {renderChildren(tag, children)}
      </UnderlinedTextLink>
    | "h1" => <Html.H1 key> {renderChildren(tag, children)} </Html.H1>
    | "h2" => <Html.H2 key> {renderChildren(tag, children)} </Html.H2>
    | "h3" => <Html.H3 key> {renderChildren(tag, children)} </Html.H3>
    | "h4" => <Html.H4 key> {renderChildren(tag, children)} </Html.H4>
    | "h5" => <Html.H5 key> {renderChildren(tag, children)} </Html.H5>
    | "h6" => <Html.H6 key> {renderChildren(tag, children)} </Html.H6>
    | "p" => <Html.P key> {renderChildren(tag, children)} </Html.P>
    | "ul" => <Html.Ul key> {renderChildren(tag, children)} </Html.Ul>
    | "li" => <Html.Li key> {renderChildren(tag, children)} </Html.Li>
    | "blockquote" =>
      <Html.BlockQuote key> {renderChildren(tag, children)} </Html.BlockQuote>
    | "br" =>
      lastSiblingHasLineBreaking := true;
      <Html.Br key />;
    | "hr" => <Html.Hr key />
    | _ =>
      ReactDOMRe.createElement(
        tag,
        ~props=
          ReactDOMRe.objToDOMProps(
            Js.Obj.empty()
            ->Js.Obj.assign({"key": key})
            ->Js.Obj.assign(props),
          ),
        [|renderChildren(tag, children)|],
      )
    }
  | Empty => React.null
  };
};

[@react.component]
let make = (~body: jsBody, ~renderChild=renderChild, ()) => {
  let tree = jsTreeToReason(body);
  <View> {renderChild("", 0, tree)} </View>;
};
