
#include <iostream>
#include <Vector/BLF.h>

#include "blfreader.h"


can_data_t open_blf_file(std::string filename)
{
	can_data_t can_data;

	Vector::BLF::File file;

	file.open(filename.c_str());

	if (!file.is_open()) {
		std::cout << "Could not open BLF file" << std::endl;
		throw blfexception();
	}

	while (file.good())
	{
		Vector::BLF::ObjectHeaderBase *ohb = nullptr;

		// read and capture exceptions, e.g. unfinished files
		try
		{
			ohb = file.read();
		}
		catch (std::runtime_error(e))
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		// skip everything other than can messages
		if (ohb == nullptr)
		{
			break;
		}

		if (ohb->objectType != Vector::BLF::ObjectType::CAN_MESSAGE)
		{
			continue;
		}

		// get message timestamp
		Vector::BLF::ObjectHeader * oh = dynamic_cast<Vector::BLF::ObjectHeader *>(ohb);
		Vector::BLF::ObjectHeader2 * oh2 = dynamic_cast<Vector::BLF::ObjectHeader2 *>(ohb);

		double ts;

		if (oh != nullptr) {
			ts = oh->objectTimeStamp;
			switch (oh->objectFlags) {
			case Vector::BLF::ObjectHeader::ObjectFlags::TimeTenMics:
				ts /= 100;
				break;
			case Vector::BLF::ObjectHeader::ObjectFlags::TimeOneNans:
				ts /= 10e9;
				break;
			}
		}
		else if (oh2 != nullptr) {
			ts = oh2->objectTimeStamp;
			switch (oh2->objectFlags) {
			case Vector::BLF::ObjectHeader2::ObjectFlags::TimeTenMics:
				ts /= 100;
				break;
			case Vector::BLF::ObjectHeader2::ObjectFlags::TimeOneNans:
				ts /= 10e9;
				break;
			}
		}

		Vector::BLF::CanMessage *msg = reinterpret_cast<Vector::BLF::CanMessage *>(ohb);

		can_data.Extended.push_back(0);
		can_data.Length.push_back(msg->dlc);
		can_data.Remote.push_back(0);
		can_data.Error.push_back(0);
		can_data.ID.push_back(msg->id);
		can_data.Timestamp.push_back(ts * 10);

		std::vector<unsigned char> msg_data(8);

		for (size_t i = 0; i < msg->dlc; i++)
		{
			msg_data[i] = msg->data[i];
		}

		std::move(msg_data.begin(), msg_data.end(), std::back_inserter(can_data.Data));

		delete msg;

		if (can_data.ID.size() % 10000 == 0)
		{
			std::cout << "Did read " << can_data.ID.size() << " CAN messages. Processing ..." << std::endl;
		}
	}

	file.close();

	return can_data;
}