#include <PresentaBALLPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLPlugin::PresentaBALLPlugin()
			: QObject(nullptr),
			  BALLPlugin(),
			  VIEWPlugin(),
			  ModularWidgetPlugin(),
			  icon_(":pluginPresentaBALL.png"),
			  html_interface_(nullptr),
			  settings_(new PresentaBALLSettings()),
			  widget_(nullptr)
		{ }

		PresentaBALLPlugin::~PresentaBALLPlugin()
		{
			// Ideally, this should be handled by Qt when destroying this plugin. However, config dialogs require a
			// QWidget as parent while this plugin is just a mere QObject.
			delete settings_;
		}

		QString PresentaBALLPlugin::getName() const
		{
			return QString("PresentaBALL");
		}

		QString PresentaBALLPlugin::getDescription() const
		{
			return QString("An interactive, HTML-based presentation environment for BALLView.");
		}

		const QPixmap* PresentaBALLPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* PresentaBALLPlugin::getConfigDialog()
		{
			return settings_;
		}

		bool PresentaBALLPlugin::activate()
		{
			if(isActive()) return true;

			if(!main_control_)
			{
				Log.error() << "PresentaBALL: Undefined main control" << std::endl;
				return false;
			}

			html_interface_ = new PresentaBALLView(settings_, main_control_);

			HTMLViewDock* html_view = new HTMLViewDock(html_interface_, main_control_, String(tr("PresentaBALL")).c_str());

			// specific settings for kiosk mode
			if (UIOperationMode::instance().getMode() > UIOperationMode::MODE_ADVANCED)
			{
				html_view->setFeatures(0);
				html_view->setMaximumWidth(730);
				html_view->setMinimumWidth(730);
			}

			main_control_->addDockWidget(Qt::LeftDockWidgetArea, html_view);

			widget_ = html_view;
			widget_->initializeWidget(*main_control_);

			return true;
		}

		bool PresentaBALLPlugin::deactivate()
		{
			if(!isActive()) return true;

			widget_->finalizeWidget(*main_control_);
			delete widget_;
			widget_ = nullptr;

			return true;
		}
	}
}
