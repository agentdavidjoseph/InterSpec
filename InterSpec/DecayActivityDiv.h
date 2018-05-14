#ifndef DecayActivityDiv_h
#define DecayActivityDiv_h
/* InterSpec: an application to analyze spectral gamma radiation data.
 
 Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC
 (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S.
 Government retains certain rights in this software.
 For questions contact William Johnson via email at wcjohns@sandia.gov, or
 alternative emails of interspec@sandia.gov, or srb@sandia.gov.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "InterSpec_config.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

#include <Wt/WEvent>
#include <Wt/WConfig.h>
#include <Wt/WContainerWidget>

#include "InterSpec/AuxWindow.h"


namespace Wt
{
  class WText;
  class WSlider;
  class WAnchor;
  class WLineEdit;
  class WCheckBox;
  class WComboBox;
  class WTabWidget;
  class WDatePicker;
  class WPushButton;
  class WDoubleSpinBox;
  class WStandardItemModel;

  namespace Chart
  {
    class WCartesianChart;
  }//namespace Chart
}//namespace Wt

namespace SandiaDecay
{
  struct Nuclide;
  class NuclideMixture;
  class SandiaDecayDataBase;
}//namespace SandiaDecay

class InterSpec;
class CsvDownloadGui;
class PeakCsvResource;
class SrbActivityChart;
class SrbActivityModel;
class DecayChainChart;
class DecaySelectNuclide;
class ChartToImageResource;
struct NuclideSelectedInfo;
class DateLengthCalculator;

#define ADD_DECAY_CHAIN_CHART 1

class DecayActivityDiv : public Wt::WContainerWidget
{
  friend class PeakCsvResource;
  friend class DateLengthCalculator;
public:
  InterSpec *m_viewer;
  
  struct Nuclide
  {
    int a;
    int z;
    int iso;
    double age;
    double activity;
    bool useCurrie;
    Wt::WContainerWidget *display;
  };//struct Nuclide

  std::vector<Nuclide>         m_nuclides;
#if( DECAY_CHART_USE_X_AXIS_DATES )
  Wt::WDatePicker             *m_initialTimePicker;
#endif
#if( DECAY_CHART_LIMIT_TIME_BY_ACTIVITY )
  Wt::WDoubleSpinBox          *m_fracActivityToEndAtEdit;
#else
  Wt::WLineEdit               *m_displayTimeLength;
#endif
  Wt::WComboBox               *m_displayActivityUnitsCombo;
  Wt::WLabel                  *m_displayActivityUnitsLabel;
  Wt::WCheckBox               *m_logYScale;
  Wt::WCheckBox               *m_photopeakLogYScale;
  Wt::WCheckBox               *m_showGridLines;
#if( DECAY_CHART_ADD_IMAGE_DOWNLOAD_LINK )
  Wt::WAnchor                 *m_pdfAnchor;
#endif

  Wt::WComboBox               *m_yAxisType;
  
  //things for the adding of activity
  Wt::WContainerWidget        *m_parentNuclidesDiv;
  Wt::WContainerWidget        *m_addParentNuclideDiv;
  Wt::WContainerWidget        *m_nuclidesAddedDiv;
  Wt::WPushButton             *m_createNewNuclideButton;

  Wt::WPushButton             *m_clearNuclidesButton;

  AuxWindow                   *m_nuclideSelectDialog;
  DecaySelectNuclide          *m_nuclideSelect;

  //Objects to actually display the chart of activities
  Wt::WTabWidget              *m_chartTabWidget;
  SrbActivityChart            *m_decayChart;
  SrbActivityModel            *m_decayModel;
  
#if( ADD_DECAY_CHAIN_CHART )
  DecayChainChart             *m_decayChainChart;
#endif
  
  Wt::Chart::WCartesianChart  *m_photoPeakChart;
  Wt::WStandardItemModel      *m_photoPeakModel;
  Wt::WSlider                 *m_photopeakAgeSlider;
  Wt::WText                   *m_sliderCurrentAgeText;
  Wt::WText                   *m_sliderEndAgeText;
  Wt::WCheckBox               *m_photoPeakYScaleFixed;

  Wt::WDoubleSpinBox          *m_photoPeakShieldingZ;
  Wt::WDoubleSpinBox          *m_photoPeakShieldingAD;

  AuxWindow                   *m_moreInfoDialog;

  Wt::WContainerWidget        *m_decayLegend;
  
#if( DECAY_CHART_ADD_IMAGE_DOWNLOAD_LINK )
  ChartToImageResource        *m_pdfResource;
#endif

  DateLengthCalculator *m_calc;
  
  CsvDownloadGui *m_csvDownloadDialog; //null if not displayed
  
  //Below are variable updated by refreshDecayDisplay() that reflect the current
  // stuff being showed by m_decayChart.  They are only used in
  // updateMouseOver(...) and can probably just be gotten rid of
  double           m_currentTimeUnits;
  double           m_currentTimeRange;
  
  int m_currentNumXPoints;

  int m_width, m_height;
  
  const SandiaDecay::SandiaDecayDataBase *m_nuclideDB;
  SandiaDecay::NuclideMixture            *m_currentMixture;


  //Functions for dealing with adding new nuclides
  void addSelectedNuclide();
  void addNuclide( const int z, const int a, const int iso,
                   const double activity, const bool useCurrie,
                   const double age );
  void addTheNuclide( const NuclideSelectedInfo &nuc );
  
  void clearAllNuclides();
  void checkTimeRangeValid();
  
  void updateYScale();
  void setPhotoPeakChartLogY( bool logy );
  void refreshDecayDisplay();
  void addDecaySeries();
  void userSetShowSeries( int series, bool show );
  
  void createCsvDownloadGui();
  void deleteCsvDownloadGui();
  void deleteCsvDownloadGuiTriggerUpdate();
  
  //TODO: call refreshPhotopeakDisplay() sepereatel from refreshDecayDisplay()
  void refreshPhotopeakDisplay();
  double photopeakSliderTime();
  void setPhopeakSliderTime( double time );

  void updateMouseOver( const Wt::WMouseEvent &event );
  void removeNuclide( Wt::WContainerWidget *frame );

  //nuclideSelectDialogDone(): makes sure none of the source Nuclides have the
  //  "EditingNuclide" style class associated with them.
  void nuclideSelectDialogDone();
  
  //sourceNuclideDoubleClicked(...): we are taking a double click by the user
  //  on a source Nuclide to mean they want to edit it.  How we keep track of
  //  this internally is by adding a "EditingNuclide" to the WContainerWidget
  //  rendering the Nuclide.  When the Nuclide div is double clicked, we will
  //  popup the nuclide select window, with the values set to those of the
  //  Nuclide.  Then when the nuclide select widget indicates to add a Nuclide
  //  by calling DecayActivityDiv::addNuclide(...) (through signal/slot), we will
  //  see if any of the source Nuclide divs have the "EditingNuclide" style
  //  class, and if so, place the new Nuclide in its place.
  //  This is a bit hacky, or not as robust as I would like, but its the easiest
  //  relatively okay way to add in this editing capability.
  void sourceNuclideDoubleClicked( Wt::WContainerWidget *w );

  double timeToDisplayTill();

  double attentuationCoeff( const double energy );

  void displayMoreInfoPopup( const double time );
  void decayChartClicked( const Wt::WMouseEvent& event );
  void photopeakDisplayMoreInfo();

  Wt::WContainerWidget *nuclideInformation(
                                    const SandiaDecay::Nuclide *nuclide ) const;
  Wt::WContainerWidget *isotopesSummary( const double time ) const;


#if( !DECAY_CHART_LIMIT_TIME_BY_ACTIVITY )
  void setTimeLimitToDisplay();
  void updatePhotopeakSliderEndDateText();
#endif

  void setDecayChartTimeRange( double dt );
  
  void setPhotopeakXScaleRange();
  void setPhotopeakYScaleRange();

  void updateInitialMixture() const;
  virtual void layoutSizeChanged( int width, int height );
  
  static double findTimeForActivityFrac( const SandiaDecay::NuclideMixture *mixture,
                                         const double fracT0ActivityWanted,
                                         const double searchStartTime = -1.0,
                                         const double searchEndTime = -1.0 );
  void deleteMoreInfoDialog();
  void manageActiveDecayChainNucStyling();

  void init();
  void initCharts();
  void setGridLineStatus();
  
  void globalKeyPressed( const Wt::WKeyEvent &e );
  
  Wt::WContainerWidget *initDisplayOptionWidgets();

  void showDecayTab();
  void showPhotopeakTab();

public:
  DecayActivityDiv( InterSpec *viewer, Wt::WContainerWidget *parent = NULL );
  virtual ~DecayActivityDiv();

  static bool dirExists( const std::string &dir );
};//DecayActivityDiv



//DecayActivityDiv_h
#endif
