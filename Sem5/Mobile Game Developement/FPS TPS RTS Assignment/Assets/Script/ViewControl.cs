using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ViewControl : MonoBehaviour, IDragHandler, IEndDragHandler {

	Vector3 initPos;
	Vector3 direction;

	public Transform rootX;
	public Transform player;

	float rotY;
	float rotX;
	float speed = 2f;

	// Use this for initialization
	void Start () {
		initPos = this.transform.position;
	}
	
	// Update is called once per frame
	void Update () {
			//x Clamp X rotation (Vertical)
			direction.y = Mathf.Clamp (direction.y, -1f, 1f);
			rotX += direction.y;
			rotX = Mathf.Clamp (rotX, -40f, 40f);

			rootX.localEulerAngles = new Vector3 (-rotX, 0f, 0f);
			Quaternion.Lerp (rootX.rotation, rootX.rotation, Time.deltaTime * speed);

			//x Y rotation (Horizontal)
			direction.x = Mathf.Clamp (direction.x, -1f, 1f);
			rotY += direction.x;
			player.localEulerAngles = new Vector3 (0f, rotY, 0f);
			Quaternion.Lerp (player.rotation, player.rotation, Time.deltaTime * speed);
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
