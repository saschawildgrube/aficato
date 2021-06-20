$this->IncludeElement("collection");

$arrayCollection = array(
	array(
		"element" => "header",
		"headerlevel" => "1",
		"header" => "A big header"
		),
	array(
		"element" => "table",
		"headerfirstrow" => true,
		"headerfirstcol" => true,
		"data" => array(
			array("row1-col1","row1-col2","row1-col3"),
			array("row2-col1","row2-col2","row2-col3"),
			array("row3-col1","row3-col2","row3-col3")
			)
		),
	array(
		"element" => "collection",
		"collection" => array(
			array(
				"element" => "header",
				"headerlevel" => "1",
				"header" => "A level 1 header within a collection"
				),
			array(
				"element" => "paragraph",
				"paragraph" => "Some text that may or may not fill afull paragraph."
				)
			)
		)
	);

$collection = new CCollectionElement($this->GetWebsite(),"default");
$strOutput = $collection->RenderCollection($arrayCollection,3);