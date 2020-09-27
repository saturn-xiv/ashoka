import React from "react";
import TextField from "@material-ui/core/TextField";
import Grid from "@material-ui/core/Grid";
import SupervisorAccountOutlined from "@material-ui/icons/SupervisorAccountOutlined";
import { useIntl } from "react-intl";
import { useForm, Controller } from "react-hook-form";
import { useDispatch } from "react-redux";

import { openSnackBar } from "../../actions";
import Application from "../../layouts/application";
import Form, { Submit, RULES } from "../../layouts/form";
import { NavLink as SignIn } from "./users/SignIn";

interface IFormInput {
  firstName: string;
  lastName: string;
  password: string;
  passwordConfirmation: string;
}

const Component = () => {
  const intl = useIntl();
  const dispatch = useDispatch();
  const { errors, control, setError, handleSubmit } = useForm();

  const onSubmit = (data: IFormInput) => {
    if (data.password !== data.passwordConfirmation) {
      setError("passwordConfirmation", {
        type: "manual",
        message: intl.formatMessage({ id: RULES.passwordConfirmation.message }),
      });
      return;
    }
    dispatch(
      openSnackBar({
        message: intl.formatMessage({ id: "flashes.success" }),
        severity: "success",
      })
    );
  };

  return (
    <Application
      icon={<SupervisorAccountOutlined />}
      title={intl.formatMessage({ id: "nut.install.title" })}
    >
      <Form onSubmit={handleSubmit(onSubmit)}>
        <Grid container spacing={2}>
          <Grid item xs={12} sm={6}>
            <Controller
              defaultValue=""
              as={TextField}
              control={control}
              rules={RULES.firstName.rule}
              variant="outlined"
              name="firstName"
              required
              fullWidth
              autoFocus
              error={errors.firstName && true}
              helperText={
                errors.firstName &&
                intl.formatMessage({ id: RULES.firstName.message })
              }
              label={intl.formatMessage({ id: "attributes.first-name" })}
            />
          </Grid>
          <Grid item xs={12} sm={6}>
            <Controller
              defaultValue=""
              as={TextField}
              control={control}
              rules={RULES.lastName.rule}
              variant="outlined"
              name="lastName"
              required
              fullWidth
              error={errors.lastName && true}
              helperText={
                errors.lastName &&
                intl.formatMessage({ id: RULES.lastName.message })
              }
              label={intl.formatMessage({ id: "attributes.last-name" })}
            />
          </Grid>
          <Grid item xs={12}>
            <Controller
              defaultValue=""
              as={TextField}
              control={control}
              rules={RULES.email.rule}
              variant="outlined"
              name="email"
              type="email"
              required
              fullWidth
              error={errors.email && true}
              helperText={
                errors.email && intl.formatMessage({ id: RULES.email.message })
              }
              label={intl.formatMessage({ id: "attributes.email" })}
            />
          </Grid>
          <Grid item xs={12}>
            <Controller
              defaultValue=""
              as={TextField}
              control={control}
              rules={RULES.password.rule}
              variant="outlined"
              name="password"
              type="password"
              required
              fullWidth
              error={errors.password && true}
              helperText={
                errors.password &&
                intl.formatMessage({ id: RULES.password.message })
              }
              label={intl.formatMessage({ id: "attributes.password" })}
            />
          </Grid>
          <Grid item xs={12}>
            <Controller
              defaultValue=""
              as={TextField}
              control={control}
              variant="outlined"
              name="passwordConfirmation"
              type="password"
              required
              fullWidth
              error={errors.passwordConfirmation && true}
              helperText={
                errors.passwordConfirmation &&
                intl.formatMessage({ id: RULES.passwordConfirmation.message })
              }
              label={intl.formatMessage({
                id: "attributes.password-confirmation",
              })}
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
