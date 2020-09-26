import React, { ReactNode } from "react";
import { makeStyles } from "@material-ui/core/styles";
import Button from "@material-ui/core/Button";
import { useIntl } from "react-intl";

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
}
const Component = (props: IProps) => {
  const classes = makeStyles((theme) => ({
    form: {
      width: "100%", // Fix IE 11 issue.
      marginTop: theme.spacing(1),
    },
  }))();
  return <form className={classes.form}>{props.children}</form>;
};

export default Component;
