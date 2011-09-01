doc = xmlRead("SCI/modules/xml/tests/unit_tests/library.xml");
    dtd = xmlDTD("SCI/modules/xml/tests/unit_tests/library.dtd");
    schema = xmlSchema("SCI/modules/xml/tests/unit_tests/library.xsd");
    rng = xmlRelaxNG("SCI/modules/xml/tests/unit_tests/library.rng");

    // We test if the document is valid
    // If no error the file is valid

    // DTD
    xmlValidate(doc, dtd);

    // Relax NG
    xmlValidate(doc, rng);

    // Schema
    xmlValidate(doc, schema);

    // All is ok... now we add a new element to the document
    doc.root.children(3) = "<a>error</a>"

    // Now the validations go to fail
    //xmlValidate(doc, dtd);
    //xmlValidate(doc, rng);
    //xmlValidate(doc, schema);

    // You can validate a document with its path
    xmlValidate("SCI/modules/xml/tests/unit_tests/library.xml")
    //xmlValidate("SCI/modules/xml/tests/unit_tests/invalid_library.xml")

    xmlValidate("SCI/modules/xml/tests/unit_tests/library.xml", schema)
    //xmlValidate("SCI/modules/xml/tests/unit_tests/invalid_library.xml", rng)

    // We close the all the open streams
    xmlClose(doc, dtd, schema, rng);