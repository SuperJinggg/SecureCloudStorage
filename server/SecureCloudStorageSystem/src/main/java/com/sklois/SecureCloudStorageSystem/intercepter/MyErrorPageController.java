package com.sklois.SecureCloudStorageSystem.intercepter;

import org.springframework.boot.web.servlet.error.ErrorController;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class MyErrorPageController implements ErrorController {

	@RequestMapping("/error")
	public String handleError() {
		// 返回错误页面的视图名称
		return "error";
	}
}
