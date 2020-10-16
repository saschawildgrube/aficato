<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayItems["COOKIE"] = $this->GetWebsite()->GetCookie("test");
		}
	}
	
