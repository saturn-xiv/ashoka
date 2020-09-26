import React from "react";
import TextField from "@material-ui/core/TextField";
import Link from "@material-ui/core/Link";
import Grid from "@material-ui/core/Grid";
import SupervisorAccountOutlined from "@material-ui/icons/SupervisorAccountOutlined";
import { useIntl } from "react-intl";

import Application from "../../layouts/application";
import Form, { Submit } from "../../layouts/form";

export default function SignUp() {
  const intl = useIntl();

  return (
    <Application
      icon={<SupervisorAccountOutlined />}
      title={intl.formatMessage({ id: "nut.install.title" })}
    >
      <Form>
        <Grid container spacing={2}>
          <Grid item xs={12} sm={6}>
            <TextField
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
            />
          </Grid>
        </Grid>
        <Submit />
        <Grid container justifyContent="flex-end">
          <Grid item>
            <Link href="#" variant="body2">
              Already have an account? Sign in
            </Link>
          </Grid>
        </Grid>
      </Form>
    </Application>
  );
}
