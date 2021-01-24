<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayItems["COOKIE"] = $this->GetWebsite()->GetCookie("test");
		}
	}
	
