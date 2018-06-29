
#include <matio.h>
#include <iostream>
#include <vector>

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

	std::cout << "converting data ..." << std::endl;
	std::vector<unsigned char> can_data_extended(can_data.Extended.begin(), can_data.Extended.end());
	std::vector<unsigned char> can_data_length(can_data.Length.begin(), can_data.Length.end());
	std::vector<unsigned char> can_data_remote(can_data.Remote.begin(), can_data.Remote.end());
	std::vector<unsigned char> can_data_error(can_data.Error.begin(), can_data.Error.end());
	std::vector<unsigned int> can_data_id(can_data.ID.begin(), can_data.ID.end());
	std::vector<double> can_data_timestamp(can_data.Timestamp.begin(), can_data.Timestamp.end());
	std::vector<unsigned char> can_data_payload(can_data.Data.begin(), can_data.Data.end());

	std::cout << "writing data ..." << std::endl;

	matvar_t *ExtendedMatVar = Mat_VarCreate(fieldnames[0], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data_extended.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[0], 0, ExtendedMatVar);

	matvar_t *LengthMatVar = Mat_VarCreate(fieldnames[1], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data_length.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[1], 0, LengthMatVar);

	matvar_t *RemoteMatVar = Mat_VarCreate(fieldnames[2], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data_remote.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[2], 0, RemoteMatVar);

	matvar_t *ErrorMatVar = Mat_VarCreate(fieldnames[3], MAT_C_UINT8, MAT_T_UINT8, 2, dim, can_data_error.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[3], 0, ErrorMatVar);

	matvar_t *IDMatVar = Mat_VarCreate(fieldnames[4], MAT_C_UINT32, MAT_T_UINT32, 2, dim, can_data_id.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[4], 0, IDMatVar);

	matvar_t *TimestampMatVar = Mat_VarCreate(fieldnames[5], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim, can_data_timestamp.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[5], 0, TimestampMatVar);


	size_t dim2[2] = { 8, can_data_extended.size() };
	matvar_t *DataMatVar = Mat_VarCreate(fieldnames[6], MAT_C_UINT8, MAT_T_UINT8, 2, dim2, can_data_payload.data(), 0);
	Mat_VarSetStructFieldByName(matstruct, fieldnames[6], 0, DataMatVar);


	Mat_VarWrite(matfp, matstruct, MAT_COMPRESSION_NONE);
	Mat_VarFree(matstruct);
	Mat_Close(matfp);

	std::cout << " ... finished!" << std::endl;
}