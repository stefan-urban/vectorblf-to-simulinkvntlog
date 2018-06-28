
#include <matio.h>

#include "vntlogwriter.h"

void write_vnt_mat(std::string filename, can_data_t can_data, std::string varname)
{
	// Open MAT file
	mat_t *matfp = NULL;
	matfp = Mat_CreateVer(filename.c_str(), NULL, MAT_FT_MAT73);


	// Basic data structure
	const char *fieldnames[] = { "Extended", "Length", "Remote", "Error", "ID", "Timestamp", "Data" };
	size_t structdim[2] = { 1, 1 };
	matvar_t* matstruct = Mat_VarCreateStruct(varname.c_str(), 2, structdim, fieldnames, 7);


	size_t dim[2] = { 1, can_data.Extended.size() };

	matvar_t *ExtendedMatVar = Mat_VarCreate(fieldnames[0], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data.Extended.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[0], 0, ExtendedMatVar);

	matvar_t *LengthMatVar = Mat_VarCreate(fieldnames[1], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data.Length.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[1], 0, LengthMatVar);

	matvar_t *RemoteMatVar = Mat_VarCreate(fieldnames[2], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data.Remote.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[2], 0, RemoteMatVar);

	matvar_t *ErrorMatVar = Mat_VarCreate(fieldnames[3], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data.Error.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[3], 0, ErrorMatVar);

	matvar_t *IDMatVar = Mat_VarCreate(fieldnames[4], MAT_C_UINT32, MAT_T_UINT32, 2, dim, can_data.ID.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[4], 0, IDMatVar);

	//double first_timestamp = Timestamp.front();
	//std::for_each(Timestamp.begin(), Timestamp.end(), [first_timestamp](double& d) { d -= first_timestamp;});

	matvar_t *TimestampMatVar = Mat_VarCreate(fieldnames[5], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim, can_data.Timestamp.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[5], 0, TimestampMatVar);


	size_t dim2[2] = { 8, can_data.Extended.size() };
	matvar_t *DataMatVar = Mat_VarCreate(fieldnames[6], MAT_C_UINT8, MAT_T_UINT8, 2, dim2, can_data.Data.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[6], 0, DataMatVar);


	Mat_VarWrite(matfp, matstruct, MAT_COMPRESSION_NONE);
	Mat_VarFree(matstruct);
	Mat_Close(matfp);
}