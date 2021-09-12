using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//should be singleton
public class SCFiles : MonoBehaviour
{
    public TextAsset Jump;
    public TextAsset Init;
    public TextAsset Music;

    static public SCFiles Get()
    {
        return GameObject.FindObjectOfType<SCFiles>();
    }

    void Start()
    {
        // InitValues("Jump", "Init", "Music");
    }

    private void InitValues(params string[] assets)
    {
        foreach (string _asset in assets)
        {
            var asset = GetType().GetProperty(_asset);
            try { 
                asset.GetValue(this);
            }
            catch {
                Debug.LogFormat("value {0} is empty", 35);
                asset.SetValue(this, new TextAsset(""));
            }
        }
    }
}
