open BsReactNative;

let imageRatio = 240. /. 350.;

let styles =
  Style.(
    StyleSheet.create({
      "list": style([alignItems(FlexStart), margin(Pt(20.))]),
      "flex": style([width(Pct(100.))]),
      "yearText":
        style([
          width(Pct(100.)),
          marginTop(Pt(20.)),
          textAlign(Center),
          fontSize(Float(22.)),
          fontWeight(`_700),
        ]),
    })
  );

[@react.component]
let make = (~talks, ()) => {
  let latestYear =
    ref(Js.Date.make()->Js.Date.getFullYear->Js.Float.toString);
  <View style=styles##list>
    {talks
     |> Array.map(item => {
          let year = item##date |> Js.String.slice(~from=0, ~to_=4);
          let newYear = year !== latestYear^;
          latestYear := year;
          <View key=item##id style=styles##flex>
            {newYear
               ? <Text style=styles##yearText> year->React.string </Text>
               : React.null}
            <TalkPreview item />
          </View>;
        })
     |> React.array}
  </View>;
};
