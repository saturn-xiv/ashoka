import React, { useEffect } from "react";
import Typography from "@material-ui/core/Typography";
import Link from "@material-ui/core/Link";
import HomeIcon from "@material-ui/icons/Home";
import Snackbar from "@material-ui/core/Snackbar";
import Alert from "@material-ui/core/Alert";
import { useSelector, useDispatch } from "react-redux";

import { refresh, IState as IApplicationState, openSnackBar } from "../actions";
import { graphql } from "../utils/request";

interface IProps {
  title: string;
}

const QUERY = `
query($title: String!, $subhead: String!, $copyright: String!) {
  about {
    version,
    languages
  },
  title: getLocale(code: $title) {
    message
  },
  subhead: getLocale(code: $subhead) {
    message
  },
  copyright: getLocale(code: $copyright) {
    message
  }
}
`;

const Component = (props: IProps) => {
  const siteInfo = useSelector((state: IApplicationState) => state.siteInfo);
  const snackBar = useSelector((state: IApplicationState) => state.snackBar);
  const dispatch = useDispatch();

  useEffect(() => {
    if (siteInfo.version === undefined) {
      graphql({
        variables: {
          title: "site.title",
          subhead: "site.subhead",
          copyright: "site.copyright",
        },
        query: QUERY,
      })
        .then((it) => {
          dispatch(
            refresh({
              version: it.about.version,
              languages: it.about.languages,
              title: it.title.message,
              subhead: it.subhead.message,
              copyright: it.copyright.message,
            })
          );
        })
        .catch((e) =>
          dispatch(
            openSnackBar({
              message: e,
              severity: "error",
            })
          )
        );
    }
  }, [dispatch, siteInfo]);

  document.title = `${props.title} | ${siteInfo.subhead} | ${siteInfo.title}`;

  const closeSnackBar = () => {
    dispatch(
      openSnackBar({
        message: undefined,
      })
    );
  };
  return (
    <>
      <Snackbar
        anchorOrigin={
          snackBar?.origin || {
            horizontal: "center",
            vertical: "top",
          }
        }
        autoHideDuration={snackBar?.timeout || 6000}
        open={snackBar?.message !== undefined}
        onClose={closeSnackBar}
      >
        <Alert variant="filled" severity={snackBar?.severity}>
          {snackBar?.message}
        </Alert>
      </Snackbar>
      <Typography variant="body2" color="textSecondary" align="center">
        {siteInfo.copyright}
        <Link
          color="inherit"
          target="_blank"
          rel="noreferrer"
          href="https://github.com/saturn-xiv/ashoka"
        >
          <HomeIcon fontSize="small" />
        </Link>
        &copy;
        {new Date().getFullYear()}
        &reg;
        {siteInfo.version}
      </Typography>
    </>
  );
};

export default Component;
