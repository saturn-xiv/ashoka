// import React from "react";

// function App() {
//   return <div className="App">Learn React</div>;
// }

// export default App;

import React from "react";
import { Provider } from "react-redux";
import { createStore } from "redux";
import { IntlProvider } from "react-intl";

import reducers from "./reducers";
import { detect as detectLocale } from "./locales";
import Router from "./Router";

const store = createStore(reducers);
const intl = detectLocale();

const App: React.FC = () => {
  return (
    <Provider store={store}>
      <IntlProvider locale={intl.locale} messages={intl.messages}>
        <Router basename="/my" />
      </IntlProvider>
    </Provider>
  );
};

export default App;
