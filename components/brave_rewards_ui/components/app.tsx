/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { bindActionCreators, Dispatch } from 'redux'
import { connect } from 'react-redux'

// Components
const RewardsPage = require('./settings')

// Utils
import * as rewardsActions from '../actions/rewards_actions'

class App extends React.Component {
  constructor (props) {
    super(props)
    this.onCreateWalletClicked = this.onCreateWalletClicked.bind(this)
  }

  onCreateWalletClicked () {
    this.actions.createWalletRequested()
  }

  get actions () {
    return this.props.actions
  }

  render () {
    const { rewardsData } = this.props
    return (
      <div>
        {
          !rewardsData.walletCreated && !rewardsData.walletCreateFailed
          ? <div>
            <a href='#' onClick={this.onCreateWalletClicked}>Yes I am in! (TODO add an actual page)</a>
          </div>
          : null
        }
        {
          rewardsData.walletCreated
          ? <RewardsPage />
          : null
        }
        {
          rewardsData.walletCreateFailed
          ? <div>Wallet Create Failed!</div>
          : null
        }
      </div>
    )
  }
}

const mapStateToProps = (state: Rewards.ApplicationState) => ({
  rewardsData: state.rewardsData
})

const mapDispatchToProps = (dispatch: Dispatch) => ({
  actions: bindActionCreators(rewardsActions, dispatch)
})

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(App)
