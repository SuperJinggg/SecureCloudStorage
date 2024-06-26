package com.sklois.SecureCloudStorageSystem.actuator;

import org.springframework.boot.actuate.endpoint.annotation.Endpoint;
import org.springframework.boot.actuate.endpoint.annotation.ReadOperation;

import java.util.HashMap;
import java.util.Map;

@Endpoint(id = "ss")
public class MyEndpoint {
    @ReadOperation
    public Map<String,String> test(){
        Map<String,String> result = new HashMap<>();
        result.put("name","ss");
        return result;
    }
}
