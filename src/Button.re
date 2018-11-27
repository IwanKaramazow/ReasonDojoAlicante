let component = ReasonReact.statelessComponent("Button");

let make = (~text, ~onClick, _children) => {
  ...component,
  render: _ =>
    <button onClick className="button"> {ReasonReact.string(text)} </button>,
};
