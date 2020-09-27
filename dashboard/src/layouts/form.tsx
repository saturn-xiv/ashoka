import React, { ReactNode } from "react";
import { makeStyles } from "@material-ui/core/styles";
import Button from "@material-ui/core/Button";
import { SnackbarOrigin } from "@material-ui/core/Snackbar";
import { useIntl } from "react-intl";

export interface IMessageBar {
  message?: string;
  timeout?: number;
  show?: boolean;
  origin?: SnackbarOrigin;
  onClose?: () => void;
}
export const MessageBarOrign: SnackbarOrigin = {
  horizontal: "center",
  vertical: "top",
};
export const MessageBarTimeout: number = 6000;

export const RULES = {
  firstName: {
    rule: { required: true, minLength: 2, maxLength: 20 },
    message: "validators.first-name",
  },
  lastName: {
    rule: { required: true, minLength: 2, maxLength: 20 },
    message: "validators.last-name",
  },
  email: {
    rule: { required: true, minLength: 2, maxLength: 32 },
    message: "validators.email",
  },
  password: {
    rule: { required: true, minLength: 6, maxLength: 32 },
    message: "validators.password",
  },
  passwordConfirmation: {
    message: "validators.password-confirmation",
  },
};

export const Submit = () => {
  const classes = makeStyles((theme) => ({
    submit: {
      margin: theme.spacing(3, 0, 2),
    },
  }))();
  const intl = useIntl();

  return (
    <Button
      type="submit"
      fullWidth
      variant="contained"
      className={classes.submit}
    >
      {intl.formatMessage({ id: "buttons.submit" })}
    </Button>
  );
};

interface IProps {
  children: ReactNode;
  onSubmit: (event: React.FormEvent<HTMLFormElement>) => void;
}
const Component = (props: IProps) => {
  const classes = makeStyles((theme) => ({
    form: {
      width: "100%", // Fix IE 11 issue.
      marginTop: theme.spacing(1),
    },
  }))();
  return (
    <form onSubmit={props.onSubmit} className={classes.form}>
      {props.children}
    </form>
  );
};

export default Component;
