using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TCP : Interface {

	// Use this for initialization
	void Start () {
        Create();
        Connect();
	}
	
	// Update is called once per frame
	void Update () {
        if (IsRun())
            OnRun();
	}

    private void OnDestroy()
    {
        Close();
    }

    public override void OnMsg(byte[] data)
    {

    }
}
