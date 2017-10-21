using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Controls : MonoBehaviour {

	const float depth = 10;


	public static Vector2 GetClickGrid () {
		Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
	
		Vector3 point = ray.origin + (ray.direction * depth);

		if (point.x > 0) {
			point.x = Mathf.Floor (point.x) + 0.5f;
		} else {
			point.x = Mathf.Ceil (point.x) - 0.5f;
		}
			

		if (point.y > 0) {
			point.y = Mathf.Floor (point.y) + 0.5f;
		} else {
			point.y = Mathf.Ceil (point.y) - 0.5f;
		}

		return point;
	}

	public static Vector2 MatchCoordinates () {
		Vector3 coord = GetClickGrid ();

		float planeLength = (GameManager.rowCount - 1.0f) / 2.0f;
		float planeHeight = (GameManager.colCount - 1.0f) / 2.0f;

		if (coord.x < 0) {
			coord.x = Mathf.Abs (Mathf.Abs(coord.x) - planeLength);
		} else {
			coord.x = coord.x + planeHeight;
		}

		if (coord.y < 0) {
			coord.y = Mathf.Abs (Mathf.Abs (coord.y) - planeHeight);
		} else {
			coord.y = coord.y + planeHeight;
		}
		return coord;
	}
}
