type status =
  | Waiting
  | Playing;

type action =
  | Start
  | Whack(int)
  | RequestNewAppearance;

type state = {
  status,
  activeIx: int,
  score: int,
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: _ => {status: Waiting, activeIx: (-1), score: 0},
  reducer: (action, state) =>
    switch (action) {
    | Start =>
      ReasonReact.UpdateWithSideEffects(
        {...state, status: Playing},
        (
          self => {
            let intervalId =
              Js.Global.setInterval(
                () => self.send(RequestNewAppearance),
                1000,
              );
            self.onUnmount(() => Js.Global.clearInterval(intervalId));
          }
        ),
      )
    | RequestNewAppearance =>
      let activeIx = Js.Math.random_int(0, 6);
      ReasonReact.Update({...state, activeIx});
    | Whack(ix) =>
      ix === state.activeIx ?
        ReasonReact.Update({
          ...state,
          activeIx: (-1),
          score: state.score + 1,
        }) :
        ReasonReact.NoUpdate
    },

  render: self => {
    let cn = (ix, activeIx) => ix === activeIx ? "item active" : "item";
    let handleClick = (ix, _) => self.send(Whack(ix));

    let score = self.state.score->string_of_int;
    <div className="container">
      <div className="header">
        <h1> {ReasonReact.string({j|$score fantas catched|j})} </h1>
      </div>
      <div className="board">
        {
          Belt.Array.range(0, 5)
          ->Belt.Array.map(ix =>
              <div
                key={string_of_int(ix)}
                className={cn(ix, self.state.activeIx)}
                onClick={handleClick(ix)}
              />
            )
          ->ReasonReact.array
        }
      </div>
      <div className="footer">
        {switch (self.state.status) {
         | Waiting =>
           <button onClick={_ => self.send(Start)}>
             {ReasonReact.string("Start!")}
           </button>
         | Playing => ReasonReact.null
         }}
      </div>
    </div>;
  },
};
