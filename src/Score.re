let component = ReasonReact.statelessComponent("Score");

let make = (~score, _children) => {
  ...component,
  render: _ => {
    let score = string_of_int(score);
    <div className="score"> {ReasonReact.string({j|Score: $score|j})} </div>;
  },
};
