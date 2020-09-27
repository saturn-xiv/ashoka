import React from "react";
import TextField from "@material-ui/core/TextField";
import FormControlLabel from "@material-ui/core/FormControlLabel";
import Checkbox from "@material-ui/core/Checkbox";
import Link from "@material-ui/core/Link";
import Grid from "@material-ui/core/Grid";
import LoginOutlined from "@material-ui/icons/LoginOutlined";
import { Link as RouterLink } from "react-router-dom";
import { useIntl } from "react-intl";

import Application from "../../../layouts/application";
import Form, { Submit } from "../../../layouts/form";
import { NavLink as SignUp } from "./SignUp";

export const NavLink = () => {
  const intl = useIntl();
  return (
    <Link component={RouterLink} to="/users/sign-in" variant="body2">
      {intl.formatMessage({ id: "nut.users.sign-in.nav" })}
    </Link>
  );
};

const Component = () => {
  const intl = useIntl();
  return (
    <Application
      icon={<LoginOutlined />}
      title={intl.formatMessage({ id: "nut.users.sign-in.title" })}
    >
      <Form onSubmit={() => {}}>
        <TextField
          variant="outlined"
          margin="normal"
          required
          fullWidth
          id="email"
          label="Email Address"
          name="email"
          autoComplete="email"
          autoFocus
        />
        <TextField
          variant="outlined"
          margin="normal"
          required
          fullWidth
          name="password"
          label="Password"
          type="password"
          id="password"
          autoComplete="current-password"
        />
        <FormControlLabel
          control={<Checkbox value="remember" color="primary" />}
          label="Remember me"
        />
        <Submit />
        <Grid container>
          <Grid item xs>
            <Link href="#" variant="body2">
              Forgot password?
            </Link>
          </Grid>
          <Grid item>
            <SignUp />
          </Grid>
        </Grid>
      </Form>
    </Application>
  );
};

export default Component;
