package com.tds;

import java.util.HashMap;
import java.util.Map;

import cn.leancloud.json.JSON;
import cn.leancloud.json.TypeReference;

public class TapCommonUE {
    public static String mapToJson(Map jsonMap) {
        return JSON.toJSONString(jsonMap);
    }

    public static Map jsonToMap(String jsonStr) {
        return JSON.parseObject(jsonStr, new TypeReference<HashMap>(){});
    }
}
