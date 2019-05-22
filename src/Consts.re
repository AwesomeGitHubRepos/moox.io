let title = {j|MoOx|j};
let defaultTitle = title ++ {j|, Freelance Front-End Web & Mobile Developer|j};
let titleTemplate = {j|%s - |j} ++ title;

let isClient: bool = [%bs.raw "typeof window !== undefined"];

let menuLinks: array(TabBar.link) = [|
  {
    link: "/",
    text: {j|Home|j},
    icon: (~width, ~height, ~fill, _) => <SVGMenuHome width height fill />,
    isActive: (current, link) => current == link,
  },
  // {
  //   link: "/solutions/",
  //   text: {j|Solutions|j},
  //   icon: (~width, ~height, ~fill, _) => <SVGMenuResume width height fill />,
  //   isActive: (current, _link) =>
  //     Js.String.startsWith("/solutions/", current),
  // },
  {
    link: "/resume/",
    text: {j|About|j},
    icon: (~width, ~height, ~fill, _) => <SVGMenuResume width height fill />,
    isActive: (current, _link) => Js.String.startsWith("/resume/", current),
  },
  {
    link: "/talks/",
    text: {j|Talks|j},
    icon: (~width, ~height, ~fill, _) => <SVGMenuTalk width height fill />,
    isActive: (current, _link) =>
      Js.String.startsWith("/talks/", current)
      || Js.String.startsWith("/talk/", current),
  },
  {
    link: "/blog/",
    text: {j|Blog|j},
    icon: (~width, ~height, ~fill, _) => <SVGMenuBlog width height fill />,
    isActive: (current, _link) => Js.String.startsWith("/blog/", current),
  },
  {
    link: "/contact/",
    text: {j|Contact|j},
    icon: (~width, ~height, ~fill, _) => <SVGMenuContact width height fill />,
    isActive: (current, link) => current == link,
  },
|];

type socialLink = {
  text: string,
  link: string,
  componentFunc: (~iconColor: string, ~iconSize: float) => React.element,
};

let socialLinks = [|
  {
    text: "@MoOx on GitHub",
    link: "https://github.com/MoOx",
    componentFunc: (~iconColor, ~iconSize) =>
      <SVGSocialGithub fill=iconColor width=iconSize height=iconSize />,
  },
  {
    text: "@MoOx on LinkedIn",
    link: "https://www.linkedin.com/in/maxthirouin/",
    componentFunc: (~iconColor, ~iconSize) =>
      <SVGSocialLinkedin fill=iconColor width=iconSize height=iconSize />,
  },
  {
    text: "@MoOx on Twitter",
    link: "https://twitter.com/MoOx",
    componentFunc: (~iconColor, ~iconSize) =>
      <SVGSocialTwitter fill=iconColor width=iconSize height=iconSize />,
  },
|];

module Colors = {
  let darkest = "hsl(219.1, 100%, 2%)";
  let dark = "hsl(219.1, 100%, 8.5%)";
  let light = "hsl(219.1, 100%, 99%)";
  let lightest = "hsl(219.1, 100%, 99.5%)";
  let tabBarIconInactive = "#909192";
  let tabBarIconActive = dark;
};
