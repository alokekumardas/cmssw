#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"

void CmsDetConstruction::buildComponent(DDFilteredView& fv, 
					GeometricDet *mother, 
					std::string attribute){
  
  //
  // at this level I check whether it is a merged detector or not
  //
  LogTrace("DetConstruction") << " CmsDetConstruction::buildComponent ";

  GeometricDet * det  = new GeometricDet(&fv,theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)));
  //Phase1 possibility: the GeometricDet can be a mergedDet
  if (theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)) ==  GeometricDet::mergedDet){

    // I have to go one step lower ...
    bool dodets = fv.firstChild(); // descend to the first Layer
    while (dodets) {
      buildSmallDetsforGlued(fv,det,attribute);
      dodets = fv.nextSibling(); // go to next layer
	/*
	Add algo to sort the merged DET
	*/
    }
    fv.parent();
  }
  //Phase2 possibility: the GeometricDet can be a stackDet - same procedure
  else if (theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)) ==  GeometricDet::OTPhase2Stack){
  
    LogTrace("DetConstruction") << " a stack ";

    bool dodets = fv.firstChild(); 
    while (dodets) {
      LogTrace("DetConstruction") << " new child! ";
      buildSmallDetsforStack(fv,det,attribute);
      dodets = fv.nextSibling(); 
    }
    fv.parent();
  }
  
  mother->addComponent(det);

}

void CmsDetConstruction::buildSmallDetsforGlued(DDFilteredView& fv, 
						GeometricDet *mother, 
						std::string attribute){

  GeometricDet * det  = 
    new GeometricDet(&fv,
		     theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)));
  static const std::string stereo = "TrackerStereoDetectors";
  if (ExtractStringFromDDD::getString(stereo,&fv) == "true"){
    uint32_t temp = 1;
    det->setGeographicalID(DetId(temp));
  }else{
    uint32_t temp = 2;
    det->setGeographicalID(DetId(temp));
  }
  
  mother->addComponent(det); 
}

void CmsDetConstruction::buildSmallDetsforStack(DDFilteredView& fv,
        	                                GeometricDet *mother,
                	                        std::string attribute){

  LogTrace("DetConstruction") << " CmsPhase2OTDetConstruction::buildSmallDetsforStacks ";
  GeometricDet * det  = new GeometricDet(&fv, theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)));
  static const std::string isLower = "TrackerLowerDetectors";
  static const std::string isUpper = "TrackerUpperDetectors";

  if (ExtractStringFromDDD::getString(isLower,&fv) == "true"){
    LogTrace("DetConstruction") << " lower ";
    uint32_t temp = 1;
    det->setGeographicalID(DetId(temp));
  } else if (ExtractStringFromDDD::getString(isUpper,&fv) == "true"){
    LogTrace("DetConstruction") << " upper ";
    uint32_t temp = 2;
    det->setGeographicalID(DetId(temp));
  } else {
    edm::LogError("DetConstruction") << " module defined in a Stack but not upper either lower!? ";
  }
  mother->addComponent(det);
}
