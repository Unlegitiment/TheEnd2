#pragma once
enum eRequestStatus {
	INACTIVE,
	REQUESTED,
	LOADING,
	LOADED,
	FAILED,
};
class fwStreamObject {
public:
	virtual void Request() = 0;
	virtual bool IsLoaded() = 0;
	virtual void Derequest() = 0;
	virtual ~fwStreamObject() = default; // WHAT THE FUCK IS THIS LANGUAGE BRUH
protected:
	eRequestStatus status = INACTIVE;
};