# Chess Game
Simple chess demo, that allows you to simulate chess game. Doesn't have AI yet or any online play. Uses SFML graphics library.

# Compilation
To compile, you could use Visual Studio, as .sln and all other needed files are already included in this git. To compile, simply download repo and create a required exe. 
You should note that, for Release version, you need to transfer Resources folder and also all SFML-2 libs (withoud -d, as -d is for debugging) to the folder with exe.
As of now, no classplatform is implemented, and current repo will only build in Windows. To build in other systems, you have to use different methods and libraries than in this repo.
You also need to install SFML, ([tutorial how to get started on Windows.](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)). This site also contains info how to get started on other operation systems.

# TODO

  - simple AI
  - threefold-repition and fifty-move-rule
  - networking and network plays
  - better UI
