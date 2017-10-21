using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class MoveControl : MonoBehaviour, IDragHandler, IEndDragHandler {

	Vector3 initPos;
	Vector3 direction;
	GameObject character;
	public float speed = 20f;
	float rotY;
	float jumpTimer;
	float dashTimer;
	bool isSwipping = false;
	bool swipeParse = false;
	bool isDash = false;
	bool isJump = false;
	Vector2 initSwipePos;
	Vector2 deltaSwipe;


	Vector3 init;
	Vector3 delta;
	float swipeLength = Screen.height / 3;

	// Use this for initialization
	void Start () {
		character = GameObject.FindGameObjectWithTag("Player");
		initPos = this.transform.position;
	}
	
	// Update is called once per frame
	void Update () {
		#region Movement
		if (ChangeSceneButtons.GetActiveScene () == "TPS") {
			direction.Normalize ();

			direction.x = Mathf.Clamp (direction.x, -1f, 1f);
			rotY += direction.x;
			character.transform.localEulerAngles = new Vector3 (0f, rotY, 0f);
			Quaternion.Lerp (character.transform.rotation, character.transform.rotation, Time.deltaTime * speed);

			character.transform.Translate (new Vector3 (direction.x, 0f, direction.y) * Time.deltaTime * speed, Space.Self);
		
			#endregion

			#region Dash
			if (!isSwipping && swipeParse) {
				isDash = true;
			} else {
				isDash = false;
			}

			if (isDash) {
				dashTimer += Time.deltaTime;
				if (dashTimer < 0.5) {
					character.transform.position += character.transform.forward * Time.deltaTime * 20f;
				} else {
					dashTimer = 0;
					isDash = false;
					swipeParse = false;
				}
			} else {
				dashTimer = 0;
			}

			#endregion

			if (Input.touchCount > 0) {

				if (Input.GetTouch (0).position.x > Screen.width / 2 && Input.GetTouch (0).position.y > Screen.height / 2) {

					#region Jump
					if (Input.GetTouch (0).tapCount == 2) {
						isJump = true;
					}

					if (isJump) {
						jumpTimer += Time.deltaTime;
						if (jumpTimer < 0.3) {
							character.transform.position += character.transform.up * Time.deltaTime * 10f;
						} else {
							jumpTimer = 0;
							isJump = false;
						}
					} else {
						jumpTimer = 0;
					}
					#endregion

					switch (Input.GetTouch (0).phase) {

					case TouchPhase.Began:
						swipeParse = false;
						initSwipePos = Input.GetTouch (0).position;
						isSwipping = true;
						break;

					case TouchPhase.Moved:
					
						deltaSwipe = initSwipePos - Input.GetTouch (0).position;
						swipeParse = false;
						isSwipping = true;
						break;

					case TouchPhase.Ended:
						if (Mathf.Abs (deltaSwipe.y) > swipeLength) {
							if (deltaSwipe.y < 0f) {
								swipeParse = true;
							}
						} else {
							swipeParse = false;
						}
						isSwipping = false;
						break;
					}

				}

			}
			
		}
	}

	public void OnDrag (PointerEventData eventData) {
		this.transform.position = eventData.position;
		direction = this.transform.position - initPos;
	}

	public void OnEndDrag (PointerEventData eventData) {
		this.transform.position = initPos;
		direction = Vector3.zero;
	}
}















