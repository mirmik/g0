#include <g0/gate.h>
#include <stdio.h>

#include <gxx/print.h>

void g0::vservice::on_input(message* msg) {
	printf("g0::vservice::on_input");
	//printf("need_to_resend");

	uint8_t type = 1;
	iovec vec[] = {
		{&type, sizeof(uint8_t)},//метка пакета данных	
		{&id, sizeof(id_t)}, //локальный id сервиса.
		{&remote_id, sizeof(id_t)}, //удаленный id сервиса.
		{&msg->stsbyte, sizeof(uint8_t)}, //байт статуса сообщения.
		{&msg->size, sizeof(uint32_t)}, //длина поля данных
		{&msg->data, msg->size}, //длина поля данных
		{nullptr, 0}
	};
	gt->send_package(vec);
}

void g0::gate::on_recv_package(const char* data, size_t sz) {
	gxx::println("on_recv_package:");
	gxx::print_dump(data,sz);
	g0::package_header* header = (g0::package_header*) data;	

	if (header->msgtype == CTRLPACK) {
		g0::control_package* control = (g0::control_package*) data;
		//control request
		printf("control request\r\n");

		if (control->header.func == CTRLFUNC_CREATE) {
			printf("open new vchannel\r\n");
			vservice* vsrvs = new vservice;
			vsrvs->remote_id = control->create.local_id;
			vsrvs->local_id = control->create.remote_id;
			vsrvs->gt = this;
			dlist_add_next(&vsrvs->chlnk, &channels);
		}
	}
}

void g0::gate::send_package(iovec* vec) {
	printf("g0::gate::send_package(iovec* vec)");
}