$this->IncludeElement("tagcloud");

$tagcloudDef = new CTagCloudDefinition();

$tagcloudDef->AddTag("Beta",0.2,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Alpha",1.0,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Gamma",0.5,$this->GetWebsite()->MakeSelfURL(),"red");
$tagcloudDef->AddTag("Delta",0.9,"","");
$tagcloudDef->AddTag("Epsilon",0.3,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Zeta",0.4,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Eta",0.0,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Theta",2,$this->GetWebsite()->MakeSelfURL(),"green");
$tagcloudDef->AddTag("Iota",0.4,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Kappa",0.8,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Lambda",0.3,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Mu",1,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Nu",0.7,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Xi",0.3,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Omicron",0.5,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Pi",3,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Rho",2,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Sigma",2,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Tau",0.3,$this->GetWebsite()->MakeSelfURL(),"");
$tagcloudDef->AddTag("Omega",0.4,"","red");


$tagcloud = new CTagCloudElement($this->GetWebsite(),"default");
$strOutput = $tagcloud->RenderTagCloud($tagcloudDef);