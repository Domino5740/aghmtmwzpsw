enum TimeUnit {SECONDS, MINUTES};

struct WatchEvent {
	enum TimeUnit eTimeUnit;
	char TimeValue;
};

void WatchInit(void);
struct WatchEvent sWatchRead(void);
