using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class ClickBlock : MonoBehaviour {

	public static bool isOverUI = false;
	// Update is called once per frame
	void Update () {
		if (EventSystem.current.IsPointerOverGameObject ()) {
			isOverUI = true;
		} else {
			isOverUI = false;
		}
	}
}
