import React, { useEffect } from "react";
import Typography from "@material-ui/core/Typography";
import Link from "@material-ui/core/Link";
import HomeIcon from "@material-ui/icons/Home";
import { Helmet } from "react-helmet";
import { useSelector, useDispatch } from "react-redux";

import { refresh, IState as IApplicationState } from "../actions";
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
      }).then((rst) => {
        if (rst.data) {
          const it = rst.data;
          dispatch(
            refresh({
              version: it.about.version,
              languages: it.about.languages,
              title: it.title.message,
              subhead: it.subhead.message,
              copyright: it.copyright.message,
            })
          );
        }
      });
    }
  }, [dispatch, siteInfo]);
  return (
    <>
      <Helmet>
        <title>
          {`${props.title} | ${siteInfo.subhead} | ${siteInfo.title}`}
        </title>
      </Helmet>
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
        {new Date().getFullYear()}.
      </Typography>
    </>
  );
};

export default Component;
