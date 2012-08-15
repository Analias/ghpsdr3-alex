#include <QtGui>

#include "hardware.h"
#include "UI.h"


DlgHardware :: DlgHardware (Connection *pC, QWidget *p): QWidget(p), pParent(p), pConn(pC)
{

}


DlgHardware :: ~DlgHardware ()
{

}


HardwarePerseus :: HardwarePerseus (Connection *pC, QWidget *pW): DlgHardware (pC, pW)
{

    // Attenuator
    QGroupBox *attGroupBox = new QGroupBox(tr("Attenuator"));
    attGroupBox->setFlat ( true );

    QRadioButton *att0Db  = new QRadioButton(tr("&0 dB"));
    QRadioButton *att10Db = new QRadioButton(tr("-&10 dB"));
    QRadioButton *att20Db = new QRadioButton(tr("-&20 dB"));
    QRadioButton *att30Db = new QRadioButton(tr("-&30 dB"));
    
    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(att0Db);
    vbox->addWidget(att10Db);
    vbox->addWidget(att20Db);
    vbox->addWidget(att30Db);
    vbox->addStretch(1);

    attGroupBox->setLayout(vbox);

    // att slider
    QSlider *pAttSlider = new QSlider(this);
    pAttSlider->setMaximum ( 0 );
    pAttSlider->setMinimum ( -40 );
    pAttSlider->setPageStep( 10 );

    // Dither
    QCheckBox *ditherCB = new QCheckBox(tr("Di&ther"));

    // Preamp
    QCheckBox *preampCB = new QCheckBox(tr("Pre&amplifier"));

    // Main layout of dialog
    QHBoxLayout *grid = new QHBoxLayout;

    // add objects
    grid->addWidget (attGroupBox);
    grid->addWidget (pAttSlider);
    grid->addWidget (ditherCB);
    grid->addWidget (preampCB);


    // use grid obecjt as main dialog's layout 
    setLayout(grid);

    // general features
    setWindowTitle(tr("Perseus"));
    //resize(240, 160);

    // interconnects
    attMapper = new QSignalMapper(this);
    connect(att0Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att0Db, 0);

    connect(att10Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att10Db, 10);

    connect(att20Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att20Db, 20);

    connect(att30Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att30Db, 30);

    connect(attMapper, SIGNAL(mapped(int)), this, SLOT(attClicked(int)));

    connect(pAttSlider, SIGNAL(valueChanged(int)), this, SLOT(attClicked(int)));

    connect(ditherCB, SIGNAL(stateChanged(int)),  this, SLOT(ditherChanged(int)));
    connect(preampCB, SIGNAL(stateChanged(int)),  this, SLOT(preampChanged(int)));

    // update the serial number in title bar
    QString command;
    command.clear(); QTextStream(&command) << "*getserial?";
    pConn->sendCommand (command);

    // defaults
    ditherCB->setCheckState(Qt::Checked);  // dither ON
    preampCB->setCheckState(Qt::Checked);  // preamp ON
    att0Db->setChecked(true);              // attenuator 0 dB
}

void HardwarePerseus :: attClicked(int state)
{
   if (state < 0) state = -state;
   qDebug() << "Attenuator: " << state << "dB";
   QString command;
   command.clear(); QTextStream(&command) << "*setattenuator " << state;
   pConn->sendCommand (command);
}

void HardwarePerseus :: ditherChanged(int state)
{
   qDebug() << "Dither: " << state;
   QString command;
   command.clear(); QTextStream(&command) << "*dither " << ((state==Qt::Checked) ? "on" : "off") ;
   pConn->sendCommand (command);
}
void HardwarePerseus :: preampChanged(int state)
{   
   qDebug() << "Preamp: " << state;
   QString command;
   command.clear(); QTextStream(&command) << "*preamp " << ((state==Qt::Checked) ? "on" : "off") ;
   pConn->sendCommand (command);
}


void HardwarePerseus :: processAnswer (QStringList list)
{
    if (list[0] == "*getserial?") {
       // try to set the serial
       qDebug() << Q_FUNC_INFO<<list[2];
       // change the title bar
       QString x;
       x.clear(); 
       QTextStream(&x) << windowTitle() << " - SN: " << list[2];

       setWindowTitle(x) ;
    }

}

HardwarePerseus :: ~HardwarePerseus ()
{

}


HardwareSdriq :: HardwareSdriq (Connection *pC, QWidget *pW): DlgHardware (pC, pW)
{

    // Attenuator
    QGroupBox *attGroupBox = new QGroupBox(tr("Attenuator"));
    attGroupBox->setFlat ( true );

    QRadioButton *att0Db  = new QRadioButton(tr("&0 dB"));
    QRadioButton *att10Db = new QRadioButton(tr("-&10 dB"));
    QRadioButton *att20Db = new QRadioButton(tr("-&20 dB"));
    QRadioButton *att30Db = new QRadioButton(tr("-&30 dB"));
    
    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(att0Db);
    vbox->addWidget(att10Db);
    vbox->addWidget(att20Db);
    vbox->addWidget(att30Db);
    vbox->addStretch(1);

    attGroupBox->setLayout(vbox);

    // Main layout of dialog
    QHBoxLayout *grid = new QHBoxLayout;

    // add objects
    grid->addWidget (attGroupBox);

    // use grid obecjt as main dialog's layout 
    setLayout(grid);

    // general features
    setWindowTitle(tr("SDR-IQ"));
    resize(240, 200);

    // interconnects
    attMapper = new QSignalMapper(this);
    connect(att0Db,  SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att0Db, 0);

    connect(att10Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att10Db, 10);

    connect(att20Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att20Db, 20);

    connect(att30Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att30Db, 30);

    connect(attMapper, SIGNAL(mapped(int)), this, SLOT(attClicked(int)));

    // update the serial number in title bar
    QString command;
    command.clear(); QTextStream(&command) << "*getserial?";
    pConn->sendCommand (command);

    // defaults
    att0Db->setChecked(true);              // attenuator 0 dB
}

void HardwareSdriq :: attClicked(int state)
{
   qDebug() << "Attenuator: " << state << "dB";
   QString command;
   command.clear(); QTextStream(&command) << "*setattenuator " << state;
   pConn->sendCommand (command);
}

void HardwareSdriq :: processAnswer (QStringList list)
{
    if (list[0] == "*getserial?") {
       // try to set the serial
       qDebug() << Q_FUNC_INFO<<list[2];
       // change the title bar
       QString x;
       x.clear(); 
       QTextStream(&x) << windowTitle() << " - SN: " << list[2];

       setWindowTitle(x) ;
    }

}

HardwareSdriq :: ~HardwareSdriq ()
{

}


HardwareHiqsdr :: HardwareHiqsdr (Connection *pC, QWidget *pW): DlgHardware (pC, pW)
{
    // Attenuator
    QGroupBox *attGroupBox = new QGroupBox(tr("Attenuator"));
    attGroupBox->setFlat ( true );

    QRadioButton *att0Db  = new QRadioButton(tr("&0 dB"));
    QRadioButton *att10Db = new QRadioButton(tr("-&10 dB"));
    QRadioButton *att20Db = new QRadioButton(tr("-&20 dB"));
    QRadioButton *att30Db = new QRadioButton(tr("-&30 dB"));
    QRadioButton *att40Db = new QRadioButton(tr("-&40 dB"));
    
    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(att0Db);
    vbox->addWidget(att10Db);
    vbox->addWidget(att20Db);
    vbox->addWidget(att30Db);
    vbox->addWidget(att40Db);
    vbox->addStretch(1);

    attGroupBox->setLayout(vbox);

    // Antenna selector
    QGroupBox *antGroupBox = new QGroupBox(tr("Antenna"));

    QRadioButton *ant0  = new QRadioButton(tr("Antenna input # &0"));
    QRadioButton *ant1  = new QRadioButton(tr("Antenna input # &1"));

    QVBoxLayout *vabox = new QVBoxLayout;

    vabox->addWidget(ant0);
    vabox->addWidget(ant1);
    antGroupBox->setLayout(vabox);


    // Main layout of dialog
    QHBoxLayout *grid = new QHBoxLayout;

    // add objects
    grid->addWidget (attGroupBox);
    grid->addWidget (antGroupBox);

    // use grid obecjt as main dialog's layout 
    setLayout(grid);

    // general features
    setWindowTitle(tr("HiQSDR"));
    resize(240, 200);

    // interconnects
    attMapper = new QSignalMapper(this);
    connect(att0Db,  SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att0Db, 0);

    connect(att10Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att10Db, 10);

    connect(att20Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att20Db, 20);

    connect(att30Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att30Db, 30);

    connect(att40Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att40Db, 40);

    connect(attMapper, SIGNAL(mapped(int)), this, SLOT(attClicked(int)));

    // antenna mapper
    antMapper = new QSignalMapper(this);
    connect(ant0,  SIGNAL(toggled(bool)), antMapper, SLOT(map()));
    antMapper->setMapping(ant0, 0);

    connect(ant1, SIGNAL(toggled(bool)), antMapper, SLOT(map()));
    antMapper->setMapping(ant1, 1);

    connect(antMapper, SIGNAL(mapped(int)), this, SLOT(antClicked(int)));

    // update the serial number in title bar
    QString command;
    command.clear(); QTextStream(&command) << "*getserial?";
    pConn->sendCommand (command);

    // defaults
    attenuatorVal = -1;
    antennaVal = -1;

    att0Db->setChecked(true);              // attenuator 0 dB
    ant0->setChecked(true);
}

void HardwareHiqsdr :: attClicked(int newVal)
{
   qDebug() << "Attenuator: " << newVal << "dB";
   if (attenuatorVal != newVal) {
      QString command;
      command.clear(); QTextStream(&command) << "*setattenuator " << newVal;
      pConn->sendCommand (command);
      attenuatorVal = newVal;
   } 
}

void HardwareHiqsdr :: antClicked(int n)
{
   qDebug() << "Antenna: " << n ;
   if (antennaVal != n) {
      QString command;
      command.clear(); QTextStream(&command) << "*selectantenna " << n;
      pConn->sendCommand (command);
      antennaVal = n;
   }
}

void HardwareHiqsdr :: processAnswer (QStringList list)
{
    if (list[0] == "*getserial?") {
       // try to set the serial
       qDebug() << Q_FUNC_INFO<<list[2];
       // change the title bar
       QString x;
       x.clear(); 
       QTextStream(&x) << windowTitle() << " - SN: " << list[2];

       setWindowTitle(x) ;
    }

}

HardwareHiqsdr :: ~HardwareHiqsdr ()
{

}


/*!
 * Set the gain for the device.
 * Manual gain mode must be enabled for this to work.
 *
 * Valid gain values (in tenths of a dB) for the E4000 tuner:
 * -10, 15, 40, 65, 90, 115, 140, 165, 190,
 * 215, 240, 290, 340, 420, 430, 450, 470, 490
 *
 * \param dev the device handle given by rtlsdr_open()
 * \param gain in tenths of a dB, 115 means 11.5 dB.
 * \return 0 on success
 */

HardwareRtlsdr :: HardwareRtlsdr (Connection *pC, QWidget *pW): DlgHardware (pC, pW)
{
    // Attenuator
    QGroupBox *attGroupBox = new QGroupBox(tr("Attenuator"));
    attGroupBox->setFlat ( true );

    QRadioButton *att1Db  = new QRadioButton(tr("-&1.0 dB"));
    QRadioButton *att15Db = new QRadioButton(tr("&1.5 dB"));
    QRadioButton *att40Db = new QRadioButton(tr("&4 dB"));
    QRadioButton *att65Db = new QRadioButton(tr("&6.5 dB"));
    QRadioButton *att90Db = new QRadioButton(tr("&9.0 dB"));
    QRadioButton *att115Db = new QRadioButton(tr("1&1.5 dB"));
    QRadioButton *att140Db = new QRadioButton(tr("1&4.0 dB"));
    QRadioButton *att165Db = new QRadioButton(tr("1&6.5 dB"));
    QRadioButton *att190Db = new QRadioButton(tr("1&9.0 dB"));
    QRadioButton *att215Db = new QRadioButton(tr("2&1.5 dB"));
    QRadioButton *att240Db = new QRadioButton(tr("2&4.0 dB"));
    QRadioButton *att290Db = new QRadioButton(tr("2&9.0 dB"));
    QRadioButton *att340Db = new QRadioButton(tr("3&4.0 dB"));
    QRadioButton *att420Db = new QRadioButton(tr("4&2.0 dB"));
    QRadioButton *att430Db = new QRadioButton(tr("4&3.0 dB"));
    QRadioButton *att450Db = new QRadioButton(tr("4&5.0 dB"));
    QRadioButton *att470Db = new QRadioButton(tr("4&7.0 dB"));
    QRadioButton *att490Db = new QRadioButton(tr("4&9.0 dB"));
    
    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(att1Db  );
    vbox->addWidget(att15Db );
    vbox->addWidget(att40Db );
    vbox->addWidget(att65Db );
    vbox->addWidget(att90Db );

    vbox->addWidget(att115Db);
    vbox->addWidget(att140Db);
    vbox->addWidget(att165Db);
    vbox->addWidget(att190Db);
    vbox->addWidget(att215Db);
    vbox->addWidget(att240Db);
    vbox->addWidget(att290Db);
    vbox->addWidget(att340Db);
    vbox->addWidget(att420Db);
    vbox->addWidget(att430Db);
    vbox->addWidget(att450Db);
    vbox->addWidget(att470Db);
    vbox->addWidget(att490Db);

    vbox->addStretch(1);

    attGroupBox->setLayout(vbox);

    // Main layout of dialog
    QHBoxLayout *grid = new QHBoxLayout;

    // add objects
    grid->addWidget (attGroupBox);

    // use grid obecjt as main dialog's layout 
    setLayout(grid);

    // general features
    setWindowTitle(tr("RTL SDR"));
    resize(240, 200);

    // interconnects
    attMapper = new QSignalMapper(this);
    connect(att1Db,  SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att1Db, -10);

    connect(att15Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att15Db, 15);

    connect(att40Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att40Db, 40);

    connect(att65Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att65Db, 65);

    connect(att90Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att90Db, 90);

    connect(att115Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att115Db, 115);

    connect(att140Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att140Db, 140);

    connect(att165Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att165Db, 165);

    connect(att190Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att190Db, 190);

    connect(att215Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att215Db, 215);

    connect(att240Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att240Db, 240);

    connect(att340Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att340Db, 340);

    connect(att420Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att420Db, 420);

    connect(att430Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att430Db, 430);

    connect(att450Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att450Db, 470);

    connect(att490Db, SIGNAL(toggled(bool)), attMapper, SLOT(map()));
    attMapper->setMapping(att490Db, 490);

    connect(attMapper, SIGNAL(mapped(int)), this, SLOT(attClicked(int)));

    // update the serial number in title bar
    QString command;
    command.clear(); QTextStream(&command) << "*getserial?";
    pConn->sendCommand (command);

    // defaults
    attenuatorVal = -1;

    att1Db->setChecked(true);              // attenuator -1 dB
}

void HardwareRtlsdr :: attClicked(int newVal)
{
   qDebug() << "Attenuator: " << newVal << "dB";
   if (attenuatorVal != newVal) {
      QString command;
      command.clear(); QTextStream(&command) << "*setattenuator " << newVal;
      pConn->sendCommand (command);
      attenuatorVal = newVal;
   } 
}

void HardwareRtlsdr :: processAnswer (QStringList list)
{
    if (list[0] == "*getserial?") {
       // try to set the serial
       qDebug() << Q_FUNC_INFO<<list[2];
       // change the title bar
       QString x;
       x.clear(); 
       QTextStream(&x) << windowTitle() << " - SN: " << list[2];

       setWindowTitle(x) ;
    }

}

HardwareRtlsdr :: ~HardwareRtlsdr ()
{

}



DlgHardware * HardwareFactory::Clone(Connection *pConn, const char *pName, QWidget * /*  p  */) 
{
   qDebug() << Q_FUNC_INFO << pName;

   if (strcmp(pName, "Perseus")==0) {
      return new HardwarePerseus(pConn, 0);
   }
   if (strcmp(pName, "SDR-IQ")==0) {
      return new HardwareSdriq(pConn, 0);
   }
   if (strcmp(pName, "HiQSDR")==0) {
      return new HardwareHiqsdr(pConn, 0);
   }
   if (strcmp(pName, "rtlsdr")==0) {
      return new HardwareRtlsdr(pConn, 0);
   }
   return 0;
}

void HardwareFactory :: processAnswer (QString a, Connection *pConn, UI *pUI )
{      
   QStringList list;

   if (a.length()) list = a.split(QRegExp(" "));

   switch (list.size()) {

   case 0:
     qDebug() << Q_FUNC_INFO<< "empty answer, delete everything";
     pUI->rmHwDlg();          
     break;

   case 1:
     qDebug() << Q_FUNC_INFO << list[0];
//     pUI->rmHwDlg();          
     break;

   case 2:
     qDebug() << Q_FUNC_INFO <<list[0] << list[1];
     break;

   case 3:
     qDebug() << Q_FUNC_INFO << list[0] << list[1] << list[2];

     if (list[0] == "*hardware?") {
        // try to activate an hardware control panel
        DlgHardware *pHwDlg = HardwareFactory::Clone (pConn, list[2].toAscii(), 0);
        if (pHwDlg) {
           qDebug() << Q_FUNC_INFO<<list[2];
           pHwDlg->show();
           pUI->setHwDlg(pHwDlg);
        }
     } else {
        DlgHardware *pHwDlg = pUI->getHwDlg();
        if (pHwDlg) {
           qDebug() << Q_FUNC_INFO<<list[2];
           emit pHwDlg->processAnswer(list);
        }
     }
     break;

   default:
     qDebug() << Q_FUNC_INFO<< "FATAL: more than 3: " << a;
   }

}

HardwareFactory hwFactory;