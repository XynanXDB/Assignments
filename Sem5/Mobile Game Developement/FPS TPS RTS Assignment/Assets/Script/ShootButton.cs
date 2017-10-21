using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public enum WEAPONTYPE {
	AK = 0,
	RPG,

	TOTAL
}

public class ShootButton : MonoBehaviour {

	private static ShootButton mInstance;
	public static ShootButton Instance {
		get {
			if (mInstance == null) {
				if (GameObject.Find ("ShootButton") != null) {
					mInstance = GameObject.Find ("ShootButton").GetComponent<ShootButton> ();
				}
			}
			return mInstance;
		}
	}

	public Transform reticuleTransform;
	public ViewControl viewControl;
	public GameObject AKMuzzleFlash;
	public GameObject AK;
	public GameObject RPG;
	public GameObject RPGBullet;
	GameObject RPGBulletGetter;
	public GameObject spawnPos;
	Vector3 reticuleWorldSpace;
	Vector3 RPGTarget;
	public static WEAPONTYPE weapon = WEAPONTYPE.AK;

	float AKShootDuration = 0.2f;
	float AKTimer;

	float RPGShootDuration = 1.0f;
	float RPGTimer;


	public bool isShot = false;

	public Transform recoil;

	float recoilSpeed = 10f;
	float recoilValue;

	void Start() {
		RPGBulletGetter = Instantiate (RPGBullet, RPG.transform) as GameObject;

		AKMuzzleFlash.SetActive (false);

		RPG.SetActive (false);
	}
		
	void Update() {
		Debug.Log (recoil.name);
			RecoilFunction ();
			if (weapon == WEAPONTYPE.AK) {
				AK.SetActive (true);
				RPG.SetActive (false);


				if (AKTimer >= AKShootDuration || isShot == false) {
					AKTimer = 0;
				}

			} else {
				AK.SetActive (false);
				RPG.SetActive (true);
				if (isShot == true) {
					RPGTimer += Time.fixedDeltaTime;
					RecoilFunction ();

					if (RPGTimer >= RPGShootDuration) {
						RPGBulletGetter = Instantiate (RPGBullet, RPG.transform) as GameObject;
						isShot = false;
						RPGTimer = 0;
					}
				}
			}
	}

	public void WhileHolding () {
		if (weapon == WEAPONTYPE.AK) {
			AKTimer += Time.fixedDeltaTime;

			if (AKTimer < AKShootDuration) {
				AKMuzzleFlash.SetActive (false);
			} else {
				ShootFunction ();
				recoilValue += 0.1f;
				AKMuzzleFlash.SetActive (true);
			}

			if (AKTimer >= AKShootDuration || isShot == false) {
				AKTimer = 0;
			}
		}
	}

	public void PointerDown () {
		if (weapon == WEAPONTYPE.AK) {
			isShot = true;
			ShootFunction ();
			recoilValue += 0.1f;
			AKMuzzleFlash.SetActive (true);
		} else {
			RPGBulletGetter.transform.parent = null;
			recoilValue += 0.5f;
			isShot = true;
		}
	}

	public void PointerUp () {
		if (weapon == WEAPONTYPE.AK) {
			isShot = false;
			AKTimer = 0;

			AKMuzzleFlash.SetActive (false);
		}
	}


	void ShootFunction () {
		if (weapon == WEAPONTYPE.AK) {
			Ray worldPosition = Camera.main.ScreenPointToRay (reticuleTransform.position);
			RaycastHit hit;

			if (Physics.Raycast (worldPosition, out hit)) {
				if (hit.transform.tag == "Enemy") {
					Destroy (hit.transform.gameObject);
				}
			}
		} 
	}

	public void RecoilFunction () {
		if (weapon == WEAPONTYPE.AK) {
			if (recoilValue > 0) {
				recoil.localRotation = Quaternion.Lerp (recoil.localRotation, Quaternion.Euler (-10, 0, 0), recoilSpeed * Time.deltaTime);
				recoilValue -= Time.deltaTime;
			} else {
				recoilValue = 0;
				recoil.localRotation = Quaternion.Lerp (recoil.localRotation, Quaternion.Euler (0, 0, 0), recoilSpeed / 2 * Time.deltaTime);
			}
		} else {
			if (recoilValue > 0) {
				recoil.localRotation = Quaternion.Lerp (recoil.localRotation, Quaternion.Euler (-15, 0, 0), recoilSpeed * Time.deltaTime);
				recoilValue -= Time.deltaTime;
			} else {
				recoilValue = 0;
				recoil.localRotation = Quaternion.Lerp (recoil.localRotation, Quaternion.Euler (0, 0, 0), recoilSpeed / 2 * Time.deltaTime);
			}
		}
	}
}
