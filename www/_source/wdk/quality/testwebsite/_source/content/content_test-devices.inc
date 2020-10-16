<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayItems["USERAGENT"] 	= $this->GetWebsite()->GetUserAgent();
			$arrayItems["DEVICETYPE"] = GetDeviceType();
			$arrayItems["ISMOBILE"] 	= RenderBool(IsDeviceMobile());
			$arrayItems["ISTABLET"] 	= RenderBool(IsDeviceTablet());
			$arrayItems["ISCOMPUTER"] = RenderBool(IsDeviceComputer());
			$arrayItems["ISWATCH"] 		= RenderBool(IsDeviceWatch());
			$arrayItems["ISTV"] 			= RenderBool(IsDeviceTV());
		}
	}
	
