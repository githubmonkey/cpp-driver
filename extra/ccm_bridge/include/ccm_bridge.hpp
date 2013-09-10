#ifndef _CASSANDRA_CCM_BRIDGE_H_INCLUDE_
#define _CASSANDRA_CCM_BRIDGE_H_INCLUDE_

#include <exception>
#include <deque>
#include <libssh2.h>

#include "configuration.hpp"
#include "esc_seq_remover.hpp"

namespace Cassandra {
	class CCMBridge {
	public:
		CCMBridge(const Configuration& settings);
		~CCMBridge();

		// Executes command on remote host
		// Returns command stdout and stderr followed by
		// shell prompth.
		std::string execute_command(const std::string& command);

		void start();
		void start(int node);
		void stop();
		void stop(int node);
		void kill();
		void kill(int node);

		void remove();
		void ring(int node);

		void bootstrap(int node, const std::string& dc = "");

		void decommission(int node);
	private:
		/* CCM functionality */
		static const std::string CCM_COMMAND;
		const std::string _ip_prefix;

		void execute_ccm_command(const std::string& ccm_args, 
								 bool use_already_existing = false);

		void execute_ccm_and_print(const std::string& ccm_args);

		/* SSH connection functionality */

		void initialize_environment();
		void wait_for_shell_prompth();

		std::string terminal_read_stdout();
		std::string terminal_read_stderr();
		std::string terminal_read(EscapeSequencesRemover& buffer, int stream);
		void terminal_read_stream(EscapeSequencesRemover& buffer, int stream);

		void terminal_write(const std::string& command);
		
		void initialize_socket_library();
		void finalize_socket_library();

		void start_connection(const Configuration& settings);
		void close_socket();
		void start_ssh_connection(const Configuration& settings);
		void close_ssh_session();

		EscapeSequencesRemover _esc_remover_stdout;
		EscapeSequencesRemover _esc_remover_stderr;

		int _socket;
		LIBSSH2_SESSION* _session;
		LIBSSH2_CHANNEL* _channel;
	};

	class CCMBridgeException : public std::exception { 
	public:
		CCMBridgeException(const char* message)
			: _message(message) { }

		virtual const char* what() const { 
			return _message;
		}
	private:
		const char* const _message;
	};
}

#endif // _CASSANDRA_CCM_BRIDGE_H_INCLUDE_