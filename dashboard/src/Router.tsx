import React, { Suspense, lazy } from "react";
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";

import routes from "./plugins";
import Loading from "./plugins/nut/Loading";

interface IProps {
  basename: string;
}

const Component = (props: IProps) => {
  return (
    <Router basename={props.basename}>
      <Suspense fallback={<Loading />}>
        <Switch>
          {routes.map((it) => (
            <Route
              key={it.path}
              path={it.path}
              exact={true}
              component={lazy(it.component)}
            />
          ))}
          <Route
            path="/"
            exact={true}
            component={lazy(() => import("./plugins/nut/Home"))}
          />
          <Route component={lazy(() => import("./plugins/nut/NotFound"))} />
        </Switch>
      </Suspense>
    </Router>
  );
};

export default Component;
