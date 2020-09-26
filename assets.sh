#!/bin/sh

set -e

# yarn create react-app dashboard --template typescript

yarn add \
    urijs @types/urijs \
    js-cookie  @types/js-cookie \
    jwt-decode @types/jwt-decode \
    lodash @types/lodash \
    dinero @types/dinero.js \
    moment moment-timezone react-moment \
    draft-js react-quill react-mde react-markdown \
    react-hook-form \
    emoji-mart google-map-react \
    qrcode.react react-copy-to-clipboard \
    react-helmet @types/react-helmet \
    react-intl \
    redux react-redux @types/react-redux \
    react-router-dom @types/react-router-dom \
    validate.js react-dropzone \
    react-syntax-highlighter @types/react-syntax-highlighter \
    @material-ui/core@next @material-ui/icons@next fontsource-roboto 

exit 0