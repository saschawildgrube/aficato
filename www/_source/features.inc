<?php
	
	function IsFeatureActive($strFeature)
	{
		$arrayActiveFeatures = array();
		
		/*
			This will explicitly activate a feature called "feature1".
			Use this function to control the global visibility of features.
			This may come handy for features that are subject to A/B testing, or
			if an implementation is yet incomplete.
		*/
		$arrayActiveFeatures["feature1"] = true;

		return GetBoolValue(ArrayGetValue($arrayActiveFeatures,$strFeature));
	}

