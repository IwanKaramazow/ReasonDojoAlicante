# Reason Dojo Alicante - Más allá del frontend

Nadie aprendió a andar en bicicleta viendo a otras personas pedalear. Las charlas son geniales, pero la realidad es que la mayoría de las personas no retiene toda la información. Aprenderemos manchándonos las manos. La meta de un Reason Dojo es precisamente construir algo en equipos pequeños.

## Editor Setup

Vscode recomendado: busca `reason-vscode` (Jared Forsyth)

https://marketplace.visualstudio.com/items?itemName=jaredly.reason-vscode

## Install

```
git clone https://github.com/IwanKaramazow/ReasonDojoAlicante

## npm
npm install
npm start
# in another terminal
npm run bundle

## yarn
yarn
yarn start
# in another terminal
yarn bundle
```

## Dojo

### 0. Hello world

```
open http://localhost:1234
```

- Open `src/App.re`
- Change "hello world" into "hello reason"
- Check the changes in your browser

### 1. Design your first ReasonReact-component

Our designer delivered the following mock:
![Button](/gh/button.png?raw=true "Optional Title")

- Create a new file `Button.re` in `src`.
- In `Button.re` create a new component

```reason
let component = ReasonReact.statelessComponent("Button");
```

- Implement the following api: `<Button text="Start game!" />`

```reason
/* Button.re */
let make = (~text, _children) => {
    ...component,
    render: _self =>
      <button className="button">
        {ReasonReact.string(text)}
      </button>
}
```

- Add an `onClick` handler that logs `start`

### 2. Build a static version of the board

![Button](/gh/board.png?raw=true "Optional Title")

Our marvelous designer has been kind, and delivered the following html:

```html
<div class="board">
  <!-- repeat 9 times - -->
  <div class="board-item">
    <div class="mole"></div>
    <div class="ground"></div>
  </div>
</div>
```

- Create a new component `Board` that renders `<Board />` in `App.re`

- _Concept_: Take a look at the following functions from Reason standard library `Belt`. Can you improve your code?

```reason
/* create array of integers */
Belt.Array.range(0, 3)

/* map over an array */
Belt.Array.map(myArray, num => num + 1)

/* pipe the result of one function into another */
myArray
->Belt.Array.filter(num => num > 5)
->Belt.Array.map(num => num + 1)
```

**TIP** Just as strings are wrapped in `ReasonReact.string`, arrays need to be wrapped in `ReasonReact.array`.

**TIP** Any normal Reason code inside render goes between braces:
`render: _ => <div> {ReasonReact.array(myArray)} </div>`

**TIP** ReasonReact uses React under the hood. Since classes are set via javascript, the following syntax needs to be used: `className="board-item"`

### 3. Static vs dynamic

Although our static version is great, the board should only be rendered as soon as the player clicked on our start button.

- At the top of `App.re` define a type state which contains the `score` (int). What is going into our state type? Anything that transitions during the time our app runs. Example: the score increases if a player hits a mole or display the board if the player clicked on the start button.

* **CONCEPT:** Define your state type as a [record](https://reasonml.github.io/docs/es-ES/record)

```reason
type person = {
  age: int,
  name: string
};

let me = {
  age: 5,
  name: "Big Reason"
};
```

- **CONCEPT:** Take a quick look at the documentation about [Variants and Constructors](https://reasonml.github.io/docs/es-ES/variant) and [Pattern Matching](https://reasonml.github.io/docs/es-ES/pattern-matching).

  ```reason
  type animal =
    | Dog
    | Cat
    | Elephant;

  let animal = Elephant;

  let remembersThings = (animal) =>
    switch (animal) {
    | Dog => false
    | Cat => false
    | Elephant => true
    };

  /* true */
  let isSmart = remembersThings(animal);
  ```

  Can you create a variant `type status` that represents whether the game has started or not? Add an extra `status` field in the `state` type that contains your variant.

### 4. It's alive!

Time to connect our state type to our `App` component!

- To declare a stateful ReasonReact component, instead of `ReasonReact.statelessComponent("MyComponentName")`, use `ReasonReact.reducerComponent("MyComponentName")`.
  Update `App.re`
- Implement a type `action` that describes state transitions in our app.

```reason
type action =
    | Start
```

**Important:** `type action` needs to be defined above `let component = ...`, the component otherwise doesn't have access to it. Reason compiles each file from top to bottom.

- Augment the components `make` function with a function that returns the initial state for your component. Example:

```reason
let make = (_children) => {
  ...component,
  /* override the initialState function, with our own implementation */
  initialState: () => {count: 0, show: true},
```

- Augment the `make` function with a `reducer` function that handles your `Start` action.
  Example:

```reason
let make = (_children) => {
    ...component,
    reducer: (action, state) =>
        switch (action) {
        | Click => ReasonReact.Update({...state, count: state.count + 1})
        | Toggle => ReasonReact.Update({...state, show: !state.show})
        },
```

**HINT** reducers return a variant `ReasonReact.Update`

- Change the `render` function of the app component to render the `<Button />` or `<Board />` based on the state:

```reason
let make = (_children) => {
    ...component,
    initialState: /* implementation */,
    reducer: /* implementation */,
    render: self =>
        <div>
            {
                switch(self.state.status) {
                    /* pattern match on the status */
                }
            }
        </div>
};
```

**Tip**: `self` is just a record that contains the state:

```reason
    type self = {
        state: state,
        send: /* function to send actions to a reducer component */
    };
```

- Instead of logging something to the console, the game should start when clicking on the start button. Change the following implementation to invoke `self.send(Start)` when the button is clicked.

```reason
<Button text="Start game!" onClick={(_click) => Js.log("start")} />
```

If you click on the start button, the board should now appear!

### 5. Moles

- modify `type state` in `App.re` to include an extra field that represents the active mole.

**TIP** don't forget to update the `initialState` function

- Pass the active mole down to the `Board` component.

Example: `<Board activeMole=self.state.activeMole />`

- Add an extra variant to `type action` to change the `activeMole` index,
  update your reducer

**TIP** Variants can contain data, with pattern matching you can extract that data

```reason
type action =
    | ChangeActiveMole(int);

switch(action) {
    | ChangeActiveMole(newIndex) => ...
}
```

- We need to start a loop that fires the `ChangeActiveMole` action.
  Take a look at the following code, can you guess how you need to change your reducer?

```reason
reducer: (state, action) => {
    | StartLoop =>
        ReasonReact.UpdateWithSideEffects(
            {...state, started: true},
            (self) => {
                let _ = Js.Global.setInterval(() => {
                    let randomInt = Js.Math.random_int(0, 9);
                    self.send(Ping(randomInt))
                }, 1000);
            }
        )
}
```

- Change the implementation of `Board`: a visible mole has the `"mole visible"` className instead of `mole`.

### 6. The sky is the limit

By now, you know enough Reason & ReasonReact to continue on your own. The sky is the limit, try to implement your own features.

Examples:

- render the score
- add an extra action that increases the score if you click on an active mole.
- add a reset button, to reset your game
