<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	abstract class CGoogleMapsReactElement extends CReactElement
	{
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID,
				$bLayoutCFG,
				$bLayoutHTML);

		}
	}

