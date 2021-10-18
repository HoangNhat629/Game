/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2jni;

import android.app.Activity;
import android.os.Bundle;
import android.os.Process;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.pm.ActivityInfo;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.common.AccountPicker;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.plus.Plus;

import android.widget.LinearLayout;
import android.widget.Toast;

public class GL2JNIActivity extends Activity implements ConnectionCallbacks, OnConnectionFailedListener {

	/* Request code used to invoke sign in user interactions. */
	private static final int RC_SIGN_IN = 0;
	/* Client used to interact with Google APIs. */
	private GoogleApiClient mGoogleApiClient;
	private boolean mIntentInProgress;
	
	GL2JNIView mView;
	private AdView adView;

	int x;
	int y;
	private static GL2JNIActivity m_istance;
	private GestureDetector gestureDetector;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		mView = new GL2JNIView(getApplication());
		m_istance = this;
		// setContentView(mView);

		adView = new AdView(this);
		adView.setAdSize(AdSize.SMART_BANNER);
		adView.setAdUnitId("ca-app-pub-1666399793767645/6314816212");

		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);

		layout.addView(adView);
		layout.addView(mView);

		AdRequest adRequest = new AdRequest.Builder().build();

		adView.loadAd(adRequest);

		setContentView(layout);

		gestureDetector = new GestureDetector(this,
				new GestureDetector.SimpleOnGestureListener() {

					@Override
					public boolean onDoubleTap(MotionEvent e) {
						GL2JNILib.ChangeEffect(0);
						return true;
					}
				});

		mGoogleApiClient = new GoogleApiClient.Builder(this)
		.addConnectionCallbacks(this)
		.addOnConnectionFailedListener(this).addApi(Plus.API)
		.addScope(Plus.SCOPE_PLUS_PROFILE).build();
	}

	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	public void onResume() {
		super.onResume();
		if (adView != null) {
			adView.resume();
		}
	}

	@Override
	public void onPause() {
		if (adView != null) {
			adView.pause();
		}
		super.onPause();
	}

	@Override
	protected void onStop() {
				super.onStop();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		mView = null;
		if (adView != null) {
			adView.destroy();
		}
		int pid = Process.myPid();
		Process.killProcess(pid);
		super.onDestroy();
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {

		final int action = event.getAction();
		final int count = event.getPointerCount();

		final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

		// final int pointerId = event.getPointerId(pointerIndex);

		if (action == MotionEvent.ACTION_MOVE)
			for (int i = 0; i < count; i++) {
				int x1 = x - (int) event.getX(i);
				int y1 = y - (int) event.getY(i);
				if ((Math.abs(x1) < 10) && (Math.abs(y1) < 10)) {
					// nothing in here
				} else {
					x = (int) event.getX(pointerIndex);
					y = (int) event.getY(pointerIndex);
					if (x1 > 0) {
					
						if (Math.abs(Math.abs(x1) - Math.abs(y1)) < 3) {
							/*if (y1 > 0) {
								GL2JNILib.rotation(1);
							} else {
								GL2JNILib.rotation(2);
							}*/
						} else {
							GL2JNILib.Move(4); //left
						}
					} else {
					
						if (Math.abs(Math.abs(x1) - Math.abs(y1)) < 3) {
							/*if (y1 > 0) {
									GL2JNILib.rotation(1);
								} else {
									GL2JNILib.rotation(2);
								}*/
						} else {
							GL2JNILib.Move(3); // right
						}
					}
				}
			}

		if (action == MotionEvent.ACTION_UP) {
			int x1 = (int) event.getX(pointerIndex);
			int y1 = (int) event.getY(pointerIndex);
			GL2JNILib.TouchEvent(1, x1, y1);
		}

		if (!gestureDetector.onTouchEvent(event))
			return super.onTouchEvent(event);
		return true;
	}

	public static void LoginGameAPI()
	{
		Log.d("cuong.nv", "LoginGameAPI android");
		m_istance.mGoogleApiClient.connect();
	}
	
	public static void LogoutGameAPI()
	{
		Log.d("cuong.nv", "LogoutGameAPI android");
		if (m_istance.mGoogleApiClient.isConnected()) {
			m_istance.mGoogleApiClient.disconnect();
		}
	}
	
	

	@Override
	public void onConnectionFailed(ConnectionResult result) {
		// TODO Auto-generated method stub
		if (!mIntentInProgress && result.hasResolution()) {
			try {
				mIntentInProgress = true;
				startIntentSenderForResult(result.getResolution()
						.getIntentSender(), RC_SIGN_IN, null, 0, 0, 0);
			} catch (SendIntentException e) {
				mIntentInProgress = false;
				//mGoogleApiClient.connect();
			}
		}
	}

	@Override
	public void onConnected(Bundle arg0) {
		Toast.makeText(this, "User is connected!", Toast.LENGTH_LONG).show();
	}

	@Override
	public void onConnectionSuspended(int arg0) {
		//mGoogleApiClient.connect();
	}
	
	protected void onActivityResult(int requestCode, int responseCode,
			Intent intent) {
			/*if (!mGoogleApiClient.isConnecting()) {
				mGoogleApiClient.connect();
			}*/
		
	}
	
	public static void LoginFaceBook()
	{
		Log.d("cuong.nv", "LoginFaceBook android");
		
	}
	
	public static void LogoutFaceBook()
	{
		Log.d("cuong.nv", "LogoutFaceBook android");
		
	}
	
}
