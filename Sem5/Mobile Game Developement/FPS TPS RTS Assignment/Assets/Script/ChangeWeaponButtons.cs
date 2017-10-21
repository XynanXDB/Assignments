using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChangeWeaponButtons : MonoBehaviour {

	public void OnClickAK () {
		ShootButton.weapon = WEAPONTYPE.AK;
	}

	public void OnClickRPG () {
		ShootButton.weapon = WEAPONTYPE.RPG;
	}
}
