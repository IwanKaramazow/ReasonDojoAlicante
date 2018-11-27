let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <div className="app">
      <div className="game"> {ReasonReact.string("hello world")} </div>
    </div>,
};