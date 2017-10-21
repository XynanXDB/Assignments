using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public static class Utils{
	static Texture2D greenTexture;
	public static Texture2D GreenTexture{
		get{
			if (greenTexture == null) {
				greenTexture = new Texture2D (1, 1);
				greenTexture.SetPixel (0, 0, Color.green);
				greenTexture.Apply ();
			}
			return greenTexture;
		}
	}
		
	public static void DrawScreenRect (Rect rect, Color color) {
		GUI.color = color;
		GUI.DrawTexture (rect, GreenTexture);
		GUI.color = Color.white;
	}

	public static void DrawScreenRectBorder (Rect rect, float stroke, Color color) {
		Utils.DrawScreenRect (new Rect (rect.xMin		  , rect.yMin		  , rect.width, stroke     ), color);
		Utils.DrawScreenRect (new Rect (rect.xMin		  , rect.yMin		  , stroke	  , rect.height), color);
		Utils.DrawScreenRect (new Rect (rect.xMax - stroke, rect.yMin		  , stroke	  , rect.height), color);
		Utils.DrawScreenRect (new Rect (rect.xMin		  , rect.yMax - stroke, rect.width, stroke     ), color);
	}

	public static Rect GetScreenRect (Vector3 pos1, Vector3 pos2) {
		pos1.y = Screen.height - pos1.y;
		pos2.y = Screen.height - pos2.y;

		Vector3 topLeft = Vector3.Min (pos1, pos2);
		Vector3 bottomRight = Vector3.Max (pos1, pos2);

		return Rect.MinMaxRect (topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}
}


public class RTSViewControl : MonoBehaviour {

	Vector3 initPos;
	bool isSelecting = false;
	public Material mat;
	public Material mat2;
	List<Renderer> unitMats;
	public List<GameObject> units;
	public Canvas canvas;
	public Text text;
	Rect rect;


	void Start () {
		units = new List<GameObject> (0);
	}
	// Update is called once per frame
	void Update () {
		Debug.Log (isSelecting);

		if (Input.touchCount > 0) {
			switch (Input.GetTouch (0).phase) {

			case TouchPhase.Began:
				units = new List<GameObject> ();
				isSelecting = true;
				initPos = Input.GetTouch (0).position;
				TouchSelect ();
				break;
			
			case TouchPhase.Moved:
				DragSelect ();
				break;

			case TouchPhase.Ended:
				isSelecting = false;
				break;
			}
		}
	}

	void OnGUI () {
		if (isSelecting) {
			Rect getRect = Utils.GetScreenRect (initPos, Input.mousePosition);
			Utils.DrawScreenRect (getRect, new Color (0.0f, 1.0f, 0.0f, 0.25f));
			Utils.DrawScreenRectBorder (getRect, 2, new Color (0.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	void TouchSelect () {
		RaycastHit hit;

		if (Physics.Raycast (Camera.main.ScreenPointToRay (Input.mousePosition), out hit)) {
			if (hit.transform.tag == "Selectables") {
				units.Add (hit.transform.gameObject);
				hit.transform.gameObject.GetComponent<Renderer> ().material = mat;
			} else if (hit.transform.tag != "Selectables") {
				foreach (GameObject go in units) {
					go.GetComponent<Renderer> ().material = mat2;
				}
			}
		}
	}
		
	void DragSelect () {
		rect = new Rect (initPos.x, initPos.y,
			Input.mousePosition.x - initPos.x, Input.mousePosition.y - initPos.y);
		
		GameObject[] items = GameObject.FindGameObjectsWithTag ("Selectables");

		for (int i = 0; i < items.Length; i++) {
			if (rect.Contains (Camera.main.WorldToScreenPoint (items [i].transform.position), true)) {
				if (units.Contains (items [i]) == false) {
					units.Add (items [i]);
					items [i].transform.gameObject.GetComponent<Renderer> ().material = mat;
				}
			} else {
				if (units.Count > 0) {
					foreach (GameObject a in items) {
						a.GetComponent<Renderer> ().material = mat2;
					}
				}
			}
		}
	}





}
