import React from "react";
import TextField from "@material-ui/core/TextField";
import FormControlLabel from "@material-ui/core/FormControlLabel";
import PersonAddOutlined from "@material-ui/icons/PersonAddOutlined";
import Checkbox from "@material-ui/core/Checkbox";
import Grid from "@material-ui/core/Grid";
import Link from "@material-ui/core/Link";
import { useIntl } from "react-intl";
import { Link as RouterLink } from "react-router-dom";

import Application from "../../../layouts/application";
import Form, { Submit } from "../../../layouts/form";
import { NavLink as SignIn } from "./SignIn";

export const NavLink = () => {
  const intl = useIntl();
  return (
    <Link component={RouterLink} to="/users/sign-up" variant="body2">
      {intl.formatMessage({ id: "nut.users.sign-up.nav" })}
    </Link>
  );
};

const Component = () => {
  const intl = useIntl();

  return (
    <Application
      icon={<PersonAddOutlined />}
      title={intl.formatMessage({ id: "nut.users.sign-up.title" })}
    >
      <Form onSubmit={() => {}}>
        <Grid container spacing={2}>
          <Grid item xs={12} sm={6}>
            <TextField
              autoComplete="fname"
              name="firstName"
              variant="outlined"
              required
              fullWidth
              id="firstName"
              label="First Name"
              autoFocus
            />
          </Grid>
          <Grid item xs={12} sm={6}>
            <TextField
              variant="outlined"
              required
              fullWidth
              id="lastName"
              label="Last Name"
              name="lastName"
              autoComplete="lname"
            />
          </Grid>
          <Grid item xs={12}>
            <TextField
              variant="outlined"
              required
              fullWidth
              id="email"
              label="Email Address"
              name="email"
              autoComplete="email"
            />
          </Grid>
          <Grid item xs={12}>
            <TextField
              variant="outlined"
              required
              fullWidth
              name="password"
              label="Password"
              type="password"
              id="password"
              autoComplete="current-password"
            />
          </Grid>
          <Grid item xs={12}>
            <FormControlLabel
              control={<Checkbox value="allowExtraEmails" color="primary" />}
              label={intl.formatMessage({ id: "nut.users.sign-up.agreed" })}
            />
          </Grid>
        </Grid>
        <Submit />
        <Grid container justifyContent="flex-end">
          <Grid item>
            <SignIn />
          </Grid>
        </Grid>
      </Form>
    </Application>
  );
};

export default Component;
