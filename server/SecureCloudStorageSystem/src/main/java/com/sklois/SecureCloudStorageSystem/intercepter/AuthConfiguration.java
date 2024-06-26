package com.sklois.SecureCloudStorageSystem.intercepter;

import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Configuration
public class AuthConfiguration implements WebMvcConfigurer {

    @Bean
    public AuthIntercepter authIntercepter(){
        return new AuthIntercepter();
    }

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        // 注册拦截器
        InterceptorRegistration loginRegistry = registry.addInterceptor(authIntercepter());
        // 拦截路径
        loginRegistry.addPathPatterns("/**");
        loginRegistry.excludePathPatterns("/userregister");
        loginRegistry.excludePathPatterns("/checkusernameexistence");
        loginRegistry.excludePathPatterns("/userauthinit");
        loginRegistry.excludePathPatterns("/userauth");
        loginRegistry.excludePathPatterns("/error");
        loginRegistry.excludePathPatterns("/error.html");
        loginRegistry.excludePathPatterns("/amazontest");
        loginRegistry.excludePathPatterns("/swifttest");
        loginRegistry.excludePathPatterns("/index.html");
        loginRegistry.excludePathPatterns("/monitor/configprops");



        loginRegistry.excludePathPatterns("/getcurrentversion");

        loginRegistry.excludePathPatterns("/baidutest");
        loginRegistry.excludePathPatterns("/teststs");


        loginRegistry.excludePathPatterns("/*.js");
        loginRegistry.excludePathPatterns("/client/SecurityCloudStorageClient.exe");

    }


}
