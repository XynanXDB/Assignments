using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class CellScript : MonoBehaviour {

	public enum Condition {
		SETTLED = 0,
		UNSETTLED,

		TOTAL
	}

	public enum CellType {
		NORMAL = 0,
		WALL,
		STARTPOINT,
		ENDPOINT,

		TOTAL
	}

	public bool isOpen = false;
	public bool isClose = false;
	public int x;
	public int y;
	public Condition condition;
	public Condition nextCondition;
	public CellType type = CellType.NORMAL;
	public List<CellScript> neighbours;
	public int f = 0;
	public int g = 0;
	public int h = 999;

	[HideInInspector] public SpriteRenderer renderer;

	void Awake () {
		renderer = GetComponent<SpriteRenderer> ();
		neighbours = new List<CellScript> ();

		condition = Condition.UNSETTLED;
		nextCondition = Condition.UNSETTLED;
	}

	public void UpdateCell () {
		condition = nextCondition;

		if (type == CellType.ENDPOINT) {
			renderer.color = Color.red;
		} else if (type == CellType.STARTPOINT) {
			renderer.color = Color.green;
		} else if (type == CellType.NORMAL) {
			if (condition == Condition.UNSETTLED && nextCondition == Condition.UNSETTLED) {
				renderer.color = Color.white;
			} else if (condition == Condition.SETTLED && nextCondition == Condition.SETTLED) {
				renderer.color = Color.magenta;
			}
		}
	}
}
