package com.lc;
import org.json.JSONObject;
import java.util.Map;

public class JavaHelperUE {
    public static String getJson(Map jsonMap) {
        JSONObject object = new JSONObject(jsonMap);
        return object.toString();
    }


}
