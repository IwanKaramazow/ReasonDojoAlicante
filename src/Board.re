let component = ReasonReact.statelessComponent("Board");

let make = (~activeIx, ~onClick, ~clickedIx, _children) => {
  ...component,

  render: _ =>
    <div className="board">
      {Belt.Array.range(0, 8)
       ->Belt.Array.map(ix => {
           Js.log(clickedIx);
           let moleClassName = ix === activeIx ? "mole visible" : "mole";

           <div key={string_of_int(ix)} className="board-item">
             <div className=moleClassName onClick={_ => onClick(ix)} />
             <div className="ground" />
             {clickedIx === ix ? <div className="hit" /> : ReasonReact.null}
           </div>;
         })
       ->ReasonReact.array}
    </div>,
};
