using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIScript : MonoBehaviour {
	
	#region Camera Variables
	float initOrthoSize = 7.31f;
	float orthoMaxZoom = 50f;
	float orthoMinZoom = 5f;
	float scrollSpeed = 0.5f;
	Vector2 panBounds = new Vector2 (-90f, 90f);
	float panSpeed = 0.5f;
	#endregion

	#region UI Variables
	public Button searchButton;
	public Button clearWallButton;
	public Button pauseButton;
	public Button modeSwitchButton;
	public Slider speedSlider;
	public Toggle isDiagonal;
	public Dropdown heuristicMenu;
	public InputField weightInput;
	public Image dropDownArrow;
	public Text generation;
	int generationCount = 0;
	bool isExpanded = false;
	RectTransform rect;
	Button menuButton;

	Vector3 startPos;
	Vector3 endPos;
	#endregion

	// Use this for initialization
	void Start () {
		heuristicMenu.onValueChanged.AddListener (delegate {
			HeuristicMenuValueChangedHandler(heuristicMenu);
		});

		weightInput.onValueChanged.AddListener (delegate {
			SetWeight();
		});

		weightInput.contentType = InputField.ContentType.IntegerNumber;
		initOrthoSize = Camera.main.orthographicSize;
		searchButton.GetComponentInChildren<Text>().text = "Search";
		rect = this.GetComponent<RectTransform> ();
		startPos = transform.localPosition;
		endPos = new Vector3 (startPos.x, startPos.y - rect.rect.height + 75.0f, startPos.z);
	}
	
	// Update is called once per frame
	void Update () {

		ZoomControl ();
		PanControl ();

		if (!isExpanded) {
			transform.localPosition = Vector3.Lerp (transform.localPosition, endPos, Time.deltaTime * 2.0f);
			dropDownArrow.transform.localRotation = Quaternion.Lerp (dropDownArrow.transform.localRotation, Quaternion.AngleAxis (180.0f, Vector3.forward), Time.deltaTime * 5.0f);
		} else {
			transform.localPosition = Vector3.Lerp (transform.localPosition, startPos, Time.deltaTime * 2.0f);
			dropDownArrow.transform.localRotation = Quaternion.Lerp (dropDownArrow.transform.localRotation, Quaternion.AngleAxis (0.0f, Vector3.forward), Time.deltaTime * 5.0f);
		}

		if (GameManagerScript.isRun == false && GameManagerScript.path == null)
			searchButton.GetComponentInChildren <Text> ().text = "Search";
		else
			searchButton.GetComponentInChildren <Text> ().text = "Restart\nSearch";

		if (GameManagerScript.isRun == false && GameManagerScript.path != null) { //x Cancel Path
			
			pauseButton.GetComponentInChildren <Text> ().text = "Clear\nPath";
		} else if (GameManagerScript.isRun == true && GameManagerScript.path == null) {
			pauseButton.interactable = true;
		} else {
			pauseButton.interactable = false;
			pauseButton.GetComponentInChildren <Text> ().text = "Pause";
		}

		if (GameManagerScript.isAStar) {
			modeSwitchButton.GetComponentInChildren<Text> ().text = "A*";
		} else {
			modeSwitchButton.GetComponentInChildren<Text> ().text = "Djikstra";
		}

		generation.text = "Generation :    " + generationCount;
	}

	void HeuristicMenuValueChangedHandler (Dropdown menu) {
		switch (menu.captionText.text) {
		case "Manhattan":
			GameManagerScript.heuristicType = HEURISTIC_TYPE.MANHATTAN;
			break;
		case "Euclidean":
			GameManagerScript.heuristicType = HEURISTIC_TYPE.EUCLIDEAN;
			break;
		case "Octile":
			GameManagerScript.heuristicType = HEURISTIC_TYPE.OCTILE;
			break;
		case "Chebyshev":
			GameManagerScript.heuristicType = HEURISTIC_TYPE.CHEBYSHEV;
			break;
		}
	}

	public void OnClickMenu () {
		if (!isExpanded)
			isExpanded = true;
		else
			isExpanded = false;
	}

	public void OnClickSearch () {

		Clear (GameManagerScript.processList);
		Clear (GameManagerScript.dumpList);
		ClearPath ();

		GameManagerScript.Instance.InitStartEndCells ();
		GameManagerScript.isEndSearch = false;
		generationCount = 0;

		GameManagerScript.processList.Add(GameManagerScript.cellMatrix[(int)GameManagerScript.Instance.startPos.x, (int)GameManagerScript.Instance.startPos.y]);
		GameManagerScript.isRun = true;

		if (GameManagerScript.path != null) {
			OnClickPause_ClearPath ();
			GameManagerScript.processList.Add(GameManagerScript.cellMatrix[(int)GameManagerScript.Instance.startPos.x, (int)GameManagerScript.Instance.startPos.y]);
		}

		StartCoroutine (SearchCoroutine ());
	}

	public void OnClickPause_ClearPath () {
		if (GameManagerScript.isRun && GameManagerScript.path == null) { 
			GameManagerScript.isRun = false;
		} else if (!GameManagerScript.isRun && GameManagerScript.path != null) { 
			Clear (GameManagerScript.processList);
			Clear (GameManagerScript.dumpList);
			ClearPath ();

			GameManagerScript.Instance.InitStartEndCells ();
			GameManagerScript.isEndSearch = false;
			generationCount = 0;
		}
	}

	public void OnClickClearWall () {

		Clear (GameManagerScript.walls);
	}

	public void OnClickToggleDiagonal () {
		if (isDiagonal.isOn) {
			GameManagerScript.isDiagonal = true;
		} else {
			GameManagerScript.isDiagonal = false;
		}
	}

	void Clear (List<CellScript> list) {
		if (list.Count > 0) {
			foreach (CellScript a in list) {
				a.condition = CellScript.Condition.UNSETTLED;
				a.nextCondition = CellScript.Condition.UNSETTLED;

				a.f = 0;
				a.g = 0;
				a.h = 0;
				a.isClose = false;
				a.isOpen = false;

				if (list == GameManagerScript.walls) {
					a.type = CellScript.CellType.NORMAL;
					a.renderer.color = Color.white;
				} else {
					if (a.type == CellScript.CellType.WALL) {
						a.renderer.color = Color.grey;
					} else {
						a.renderer.color = Color.white;
					}
				}
					

				if (a.transform.childCount > 0) {
					a.transform.DetachChildren();
				}
			}
			list.Clear ();
		}
	}

	public void OnClickAStarOrDjikstra () {
		if (GameManagerScript.isAStar) {
			GameManagerScript.isAStar = false;
			heuristicMenu.interactable = false;
		} else {
			GameManagerScript.isAStar = true;
			heuristicMenu.interactable = true;
		}
	}

	private void SetWeight () {
		if (GameManagerScript.isAStar) {
			weightInput.interactable = true;
			GameManagerScript.Instance.weight = int.Parse(weightInput.text);
		} else {
			weightInput.interactable = false;
		}
	}


	void ClearPath (){
		if (GameManagerScript.path != null) {
			foreach (CellScript a in GameManagerScript.path) {
				a.condition = CellScript.Condition.UNSETTLED;
				a.nextCondition = CellScript.Condition.UNSETTLED;

				a.f = 0;
				a.g = 0;
				a.h = 0;
				a.isClose = false;
				a.isOpen = false;

				if (a.type == CellScript.CellType.WALL) {
					a.renderer.color = Color.grey;
				} else {
					a.renderer.color = Color.white;
				}
				if (a.transform.childCount > 0) {
					a.transform.DetachChildren ();
				}
			}
			GameManagerScript.path = null;

			for (int i = 0; i < GameManagerScript.Instance.lineRenderer.positionCount; i++) {
				GameManagerScript.Instance.lineRenderer.SetPosition (i, Vector3.zero);
			}
		}
	}

	IEnumerator SearchCoroutine () {
		while (GameManagerScript.isRun) {
			GameManagerScript.Instance.UpdateProcessList ();
			yield return new WaitForSeconds (speedSlider.value);
			GameManagerScript.Instance.ApplyUpdateOnProcessList ();
			generationCount++;

			if (GameManagerScript.path != null) {
				GameManagerScript.isRun = false;
			}
		}

		if (GameManagerScript.isRun == false) {
			StopCoroutine (SearchCoroutine ());
		}
	}

	void ZoomControl () {

		if (Input.GetAxis ("Mouse ScrollWheel") < 0 && Camera.main.orthographicSize < orthoMaxZoom) {
			Camera.main.orthographicSize = Camera.main.orthographicSize + scrollSpeed;
		}

		if (Input.GetAxis ("Mouse ScrollWheel") > 0 && Camera.main.orthographicSize > orthoMinZoom) {
			Camera.main.orthographicSize = Camera.main.orthographicSize - scrollSpeed;

		}
	}

	void PanControl () {
		if (Input.GetKey (KeyCode.W) && Camera.main.transform.position.y < panBounds.y) {
			Camera.main.transform.position += (Vector3.up * panSpeed);
		}

		if (Input.GetKey (KeyCode.S) && Camera.main.transform.position.y > panBounds.x) {
			Camera.main.transform.position += (Vector3.down * panSpeed);
		}

		if (Input.GetKey (KeyCode.A) && Camera.main.transform.position.x > panBounds.x) {
			Camera.main.transform.position += (Vector3.left * panSpeed);
		}

		if (Input.GetKey (KeyCode.D) && Camera.main.transform.position.x < panBounds.y) {
			Camera.main.transform.position += (Vector3.right * panSpeed);
		}
	}
}
