using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIScript : MonoBehaviour {
	
	RectTransform rect;
	Button menuButton;
	public Slider zoomSlider;
	public Slider speedSlider;
	public Button runButton;
	public Image dropDownArrow;
	public Text generation;
	int generationCount = 0;
	Vector3 startPos;
	Vector3 endPos;
	float initOrthoSize;
	bool isExpanded = false;

	// Use this for initialization
	void Start () {
		initOrthoSize = Camera.main.orthographicSize;
		runButton.GetComponentInChildren<Text> ().text = "Run";
		rect = this.GetComponent<RectTransform> ();
		startPos = transform.localPosition;
		endPos = new Vector3 (startPos.x, startPos.y - rect.rect.height + 50.0f, startPos.z);
	}
	
	// Update is called once per frame
	void Update () {
		if (!isExpanded) {
			transform.localPosition = Vector3.Lerp (transform.localPosition, endPos, Time.deltaTime * 2.0f);
			dropDownArrow.transform.localRotation = Quaternion.Lerp (dropDownArrow.transform.localRotation, Quaternion.AngleAxis(180.0f, Vector3.forward), Time.deltaTime * 5.0f);
		} else {
			transform.localPosition = Vector3.Lerp (transform.localPosition, startPos, Time.deltaTime * 2.0f);
			dropDownArrow.transform.localRotation = Quaternion.Lerp (dropDownArrow.transform.localRotation, Quaternion.AngleAxis(0.0f, Vector3.forward), Time.deltaTime * 5.0f);
		}

		if (GameManager.isRunning == false) {
			runButton.GetComponentInChildren<Text> ().text = "Run";
		} else {
			runButton.GetComponentInChildren<Text>().text = "Stop";
		}
		generation.text = "Generation:     " + generationCount;
		Camera.main.orthographicSize = zoomSlider.value;
	}

	public void OnClickMenu () {
		if (!isExpanded)
			isExpanded = true;
		else
			isExpanded = false;
	}

	public void OnClickRun () {
		if (GameManager.isRunning == false) {
			GameManager.isRunning = true;
			StartCoroutine (RunCoroutine());

		} else {
			GameManager.isRunning = false;
		}
	}

	IEnumerator RunCoroutine () {
		while (GameManager.isRunning) {
			GameManager.Instance.UpdateCell ();
			generationCount++;
			yield return new WaitForSeconds (speedSlider.value);
		}

		if (GameManager.isRunning == false) {
			StopCoroutine (RunCoroutine ());
		}
	}

	public void OnClickClear () {
		foreach (Cell a in GameManager.activatedCells) {
			a.nextCondition = Cell.Condition.DEAD;
			a.condition = Cell.Condition.DEAD;
			generationCount = 0;
			a.neighbours.Clear ();

			for (int i = 0; i < a.deadNeighbours.Count; i++) {
				a.deadNeighbours [i].deadNeighboursSurroundings.Clear ();
			}

			a.deadNeighbours.Clear ();
			a.aliveCellsAroundActivatedCells = 0;
			a.aliveCellsAroundDeadCells = 0;
			a.ChangeColor ();

		}

		GameManager.activatedCells.Clear ();
		GameManager.toBeResurrected.Clear ();
	}
}
