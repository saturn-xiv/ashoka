import { IRoute } from "..";

const routes: IRoute[] = [
  {
    path: "/users/sign-in",
    component: () => import("./users/SignIn"),
  },
  {
    path: "/users/sign-up",
    component: () => import("./users/SignUp"),
  },
  {
    path: "/install",
    component: () => import("./Install"),
  },
];

export default routes;
