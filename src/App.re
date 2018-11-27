type game =
  | Waiting
  | Playing;

type action =
  | StartGame
  | Hit(int)
  | Appear(int);

type state = {
  game,
  score: int,
  activeIx: int,
  clickedIx: int,
};

let component = ReasonReact.reducerComponent("App");

let computeTimeout = score => 1000 - 20 * score;

let make = _children => {
  ...component,

  initialState: _ => {
    game: Waiting,
    score: 0,
    activeIx: (-1),
    clickedIx: (-1),
  },

  reducer: (action, state) =>
    switch (action) {
    | StartGame =>
      ReasonReact.UpdateWithSideEffects(
        {...state, game: Playing, score: 0, clickedIx: (-1)},
        (
          self =>
            Js.Global.setTimeout(
              () => {
                let nextActive = Js.Math.random_int(0, 9);
                self.send(Appear(nextActive));
              },
              computeTimeout(self.state.score),
            )
            ->ignore
        ),
      )
    | Appear(next) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, activeIx: next, clickedIx: (-1)},
        (
          self =>
            Js.Global.setTimeout(
              () => {
                let nextActive = Js.Math.random_int(0, 9);
                self.send(Appear(nextActive));
              },
              computeTimeout(self.state.score),
            )
            ->ignore
        ),
      )

    | Hit(index) =>
      if (index === state.activeIx) {
        ReasonReact.Update({
          ...state,
          score: state.score + 1,
          clickedIx: index,
        });
      } else {
        ReasonReact.NoUpdate;
      }
    },

  render: self =>
    <div className="app">
      {
        switch (self.state.game) {
        | Waiting =>
          <Button text="Start game!" onClick=(_ => self.send(StartGame)) />
        | Playing =>
          <>
            <Board
              onClick=(index => self.send(Hit(index)))
              activeIx={self.state.activeIx}
              clickedIx={self.state.clickedIx}
            />
            <Score score={self.state.score} />
          </>
        }
      }
    </div>,
};