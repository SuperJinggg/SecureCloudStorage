package com.sklois.SecureCloudStorageSystem;

import com.sklois.SecureCloudStorageSystem.actuator.MyEndpoint;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.apache.catalina.Context;
import org.apache.catalina.connector.Connector;
import org.apache.tomcat.util.descriptor.web.SecurityCollection;
import org.apache.tomcat.util.descriptor.web.SecurityConstraint;
import org.springframework.boot.web.embedded.tomcat.TomcatServletWebServerFactory;
import org.springframework.context.annotation.Configuration;


//@Configuration
@SpringBootApplication
public class SecureCloudStorageSystemApplication {

    public static void main(String[] args) {
        SpringApplication.run(SecureCloudStorageSystemApplication.class, args);
    }


    @Bean
    public Connector connector() {
        Connector connector = new Connector("org.apache.coyote.http11.Http11NioProtocol");
        connector.setScheme("http");
        connector.setPort(9090);
        connector.setSecure(false);
        connector.setRedirectPort(9443);
        return connector;
    }

    @Bean
    public TomcatServletWebServerFactory tomcatServletWebServerFactory(Connector connector) {
        TomcatServletWebServerFactory tomcat = new TomcatServletWebServerFactory() {
            @Override
            protected void postProcessContext(Context context) {
                SecurityConstraint securityConstraint = new SecurityConstraint();
                securityConstraint.setUserConstraint("CONFIDENTIAL");
                SecurityCollection collection = new SecurityCollection();
                collection.addPattern("/*");
                securityConstraint.addCollection(collection);
                context.addConstraint(securityConstraint);
            }
        };
        tomcat.addAdditionalTomcatConnectors(connector);

        return tomcat;
    }


    /**
     * 文件上传配置
     * @return
     */
//    @Bean
//    public MultipartConfigElement multipartConfigElement() {
//        MultipartConfigFactory factory = new MultipartConfigFactory();
//        //文件最大
//        factory.setMaxFileSize(DataSize.parse("10240KB"));
//        factory.setMaxRequestSize(DataSize.parse("1024000KB"));
////        factory.setMaxFileSize("10240MB"); //KB,MB
//        /// 设置总上传数据总大小
////        factory.setMaxRequestSize("102400MB");
//        return factory.createMultipartConfig();
//    }

    @Configuration
    static class MyEndpointConfiguration {
        @Bean
        @ConditionalOnMissingBean
        public MyEndpoint myendpoint() {
            return new MyEndpoint();
        }
    }
}
