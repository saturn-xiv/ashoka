import { ActionTypes, ISnackBar, SNACK_BAR_OPEN } from "../actions";

const initialState: ISnackBar = {};

export default (state = initialState, action: ActionTypes): ISnackBar => {
  switch (action.type) {
    case SNACK_BAR_OPEN:
      return Object.assign({}, state, action.payload);
    default:
      return state;
  }
};
